#include "StyleableElement.h"
#include "../Base/Tools/StringUtils.h"
#include "../Base/Logging.h"

namespace Sgl
{
	struct EraseSavedStateHandler
	{
		StyleableElement& Source;

		void operator()(StyleableElement& sender, EventArgs e) const
		{
			std::erase_if(Source._savedStates, [&sender](const auto& action)
			{
				return action.GetTarget() == &sender;
			});
		}

		bool operator==(const EraseSavedStateHandler& other) const
		{
			return &Source == &other.Source;
		}
	};

	StyleableElement::StyleableElement()
	{
		PseudoClasses.Changed += [this](PseudoClassesSet& sender, EventArgs e)
		{
			if(PseudoClasses.IsEmpty())
			{
				RestoreBaseState();
				ClearMatchingStateStyles();
				return;
			}

			auto matchedStyles = std::move(_matchingStateStyles);
			bool match = MatchStateStyles();

			if(matchedStyles != _matchingStateStyles)
			{
				RestoreBaseState();

				if(match)
				{
					SaveBaseState();
					ApplyStateStyle();
				}
			}
		};
	}

	StyleableElement::StyleableElement(StyleableElement&& other) noexcept:
		BindableObject(std::move(other)),
		Name(std::move(other.Name)),
		PseudoClasses(std::move(other.PseudoClasses)),
		Styles(std::move(other.Styles)),
		_logicalChildren(std::move(other._logicalChildren)),
		_classList(std::move(other._classList)),
		_stylingParent(std::exchange(other._stylingParent, nullptr)),
		_isAttachedToLogicalTree(other._isAttachedToLogicalTree),
		_styles(std::move(other._styles))
	{}

	void StyleableElement::SetClasses(std::string_view classNames)
	{
		_classList = SplitString(classNames, ' ');
		OnStyleClassesChanged();
	}

	void StyleableElement::SetClasses(std::vector<std::string> classList)
	{
		_classList = std::move(classList);
		OnStyleClassesChanged();
	}

	const std::vector<std::string>& StyleableElement::GetClasses() const
	{
		return _classList;
	}

	void StyleableElement::SetParent(IStyleHost* parent)
	{
		_stylingParent = parent;
	}

	void StyleableElement::ApplyStyle()
	{
		for(auto style : _styles)
		{
			style->Apply(*this, ValueSource::Style);
		}

		for(auto child : _logicalChildren)
		{
			child->ApplyStyle();
		}
	}

	void StyleableElement::OnAttachedToLogicalTree()
	{
		_isAttachedToLogicalTree = true;
		AttachedToLogicalTree.Invoke(*this);

		if(FetchStyles())
		{
			ApplyStyle();

			if(!PseudoClasses.IsEmpty() && MatchStateStyles())
			{
				SaveBaseState();
				ApplyStateStyle();
			}
		}

		for(auto child : _logicalChildren)
		{
			child->OnAttachedToLogicalTree();
		}
	}

	void StyleableElement::OnDetachedFromLogicalTree()
	{
		_isAttachedToLogicalTree = false;

		if(!PseudoClasses.IsEmpty())
		{
			RestoreBaseState();
			ClearMatchingStateStyles();
		}

		for(auto child : _logicalChildren)
		{
			child->OnDetachedFromLogicalTree();
		}

		DetachedFromLogicalTree.Invoke(*this);
	}

	void StyleableElement::OnDataContextChanged(const Ref<INotifyPropertyChanged>& dataContext)
	{
		for(auto child : GetLogicalChildren())
		{
			child->SetDataContext(dataContext, ValueSource::Inheritance);
		}
	}

	void StyleableElement::AddLogicalChild(StyleableElement* child)
	{
		_logicalChildren.push_back(child);
		child->SetParent(this);

		if(IsAttachedToLogicalTree())
		{
			child->OnAttachedToLogicalTree();
		}
	}

	void StyleableElement::RemoveLogicalChild(StyleableElement* child)
	{
		std::erase(_logicalChildren, child);
		child->SetParent(nullptr);

		if(IsAttachedToLogicalTree())
		{
			child->OnDetachedFromLogicalTree();
		}
	}

	bool StyleableElement::FetchStyles()
	{
		_styles.clear();
		_stateStyles.clear();

		std::vector<const StyleCollection*> stylesCollections;
		stylesCollections.reserve(4);
		stylesCollections.push_back(&Styles);

		auto parent = _stylingParent;
		while(parent != nullptr)
		{
			stylesCollections.push_back(&parent->GetStyles());
			parent = parent->GetStylingParent();
		}
		
		for(auto it = stylesCollections.rbegin(); it != stylesCollections.rend(); ++it)
		{
			FetchStylesFrom(**it);
		}

		return !_styles.empty() || !_stateStyles.empty();
	}

	void StyleableElement::FetchStylesFrom(const StyleCollection& styles)
	{
		if(styles.IsEmpty())
		{
			return;
		}

		for(auto& style : styles)
		{
			if(style.Selector.Match(*this))
			{
				if(style.Selector.HasState())
				{
					_stateStyles.push_back(&style);
				}
				else
				{
					_styles.push_back(&style);
				}
			}
		}
	}

	void StyleableElement::OnStyleClassesChanged()
	{
		if(!IsAttachedToLogicalTree())
		{
			return;
		}

		if(FetchStyles())
		{
			ApplyStyle();
		}
	}

	void StyleableElement::ApplyStateStyle()
	{
		for(auto style : _matchingStateStyles)
		{
			style->Apply(*this, ValueSource::PseudoClass);
		}
	}

	void StyleableElement::SaveBaseState()
	{
		for(auto style : _matchingStateStyles)
		{
			auto& target = style->SelectTarget(*this);

			for(auto& setter : style->_setters)
			{
				auto& property = setter->GetProperty();
				auto restore = property.CreateRestoreAction(&target);

				StyleableElementEventHandler detachedHandler;
				if(this != &target)
				{
					detachedHandler = EraseSavedStateHandler(*this);
					target.DetachedFromLogicalTree += detachedHandler;				
				}
				
				_savedStates.emplace_back(
					std::move(restore),
					&target,
					std::move(detachedHandler)
				);
			}
		}
	}

	void StyleableElement::RestoreBaseState()
	{
		_savedStates.clear();
	}

	void StyleableElement::ClearMatchingStateStyles()
	{
		_matchingStateStyles.clear();
	}

	bool StyleableElement::MatchStateStyles()
	{
		_matchingStateStyles.clear();

		for(auto style : _stateStyles)
		{
			if(style->Selector.MatchState(*this))
			{
				_matchingStateStyles.push_back(style);
			}
		}

		return _matchingStateStyles.size() > 0;
	}

	StyleableElement::SavedState::SavedState(Action<> restore,
										     StyleableElement* target, 
										     StyleableElementEventHandler detachedHandler):
		_restore(std::move(restore)),
		_target(target),
		_detachedHandler(std::move(detachedHandler))
	{}

	StyleableElement::SavedState::SavedState(SavedState&& other) noexcept:
		_restore(std::move(other._restore)),
		_target(std::exchange(other._target, nullptr)),
		_detachedHandler(std::move(other._detachedHandler))
	{}

	StyleableElement::SavedState::~SavedState()
	{
		if(_restore.HasTarget())
		{
			_restore();
		}

		if(_detachedHandler.HasTarget())
		{
			_target->DetachedFromLogicalTree -= _detachedHandler;
		}
	}

	StyleableElement* StyleableElement::SavedState::GetTarget() const noexcept
	{
		return _target;
	}

	StyleableElement::SavedState& StyleableElement::SavedState::operator=(SavedState&& other) noexcept
	{
		if(this != &other)
		{
			_restore = std::move(other._restore);
			_target = std::exchange(other._target, nullptr);
			_detachedHandler = std::move(other._detachedHandler);
		}

		return *this;
	}
}