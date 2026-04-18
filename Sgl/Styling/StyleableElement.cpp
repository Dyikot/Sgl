#include "StyleableElement.h"
#include "../Base/Tools/StringUtils.h"
#include "../Base/Logging.h"

namespace Sgl
{
	StyleableElement::StyleableElement()
	{
		PseudoClasses.Changed += [this](PseudoClassesSet& sender, EventArgs e)
		{
			if(PseudoClasses.IsEmpty())
			{
				ClearAndRestoreBaseState();
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
	}

	void StyleableElement::OnDetachedFromLogicalTree()
	{
		_isAttachedToLogicalTree = false;

		if(!PseudoClasses.IsEmpty())
		{
			ClearAndRestoreBaseState();
		}

		DetachedFromLogicalTree.Invoke(*this);
	}

	bool StyleableElement::FetchStyles()
	{
		_styles.clear();
		_stateStyles.clear();

		std::vector<const StyleCollection*> stylesCollections;
		stylesCollections.reserve(4);
		stylesCollections.push_back(&Styles);

		if(auto parent = _stylingParent)
		{
			while(parent != nullptr)
			{
				stylesCollections.push_back(&parent->GetStyles());
				parent = parent->GetStylingParent();
			}
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

	struct ClearBaseStateHandler
	{
		StyleableElement& Source;

		void operator()(StyleableElement& sender, EventArgs e) const
		{
			std::erase_if(Source._restoreStateActions, [&sender](const auto& action)
			{
				return action.Target == &sender;
			});
		}

		bool operator==(const ClearBaseStateHandler& other) const
		{
			return &Source == &other.Source;
		}
	};

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
			bool hasProjection = style->Projection.HasTarget();
			auto& target = hasProjection ? style->Projection(*this) : *this;

			for(auto& setter : style->_setters)
			{
				auto& property = setter->GetProperty();
				auto restore = property.CreateRestoreAction(&target);

				if(hasProjection)
				{
					StyleableElementEventHandler detachedHandler = ClearBaseStateHandler(*this);
					target.DetachedFromLogicalTree += detachedHandler;

					_restoreStateActions.emplace_back(
						std::move(restore),
						&target,
						std::move(detachedHandler)
					);
				}
				else
				{
					_restoreStateActions.emplace_back(
						std::move(restore),
						&target,
						nullptr
					);
				}
			}
		}
	}

	void StyleableElement::RestoreBaseState()
	{
		for(auto& restoreState : _restoreStateActions)
		{
			restoreState.Restore();
		}
	}

	bool StyleableElement::MatchStateStyles()
	{
		_matchingStateStyles.clear();

		for(auto& style : _stateStyles)
		{
			if(style->Selector.MatchState(*this))
			{
				_matchingStateStyles.push_back(style);
			}
		}

		return _matchingStateStyles.size() > 0;
	}

	void StyleableElement::ClearAndRestoreBaseState()
	{
		for(auto& restoreState : _restoreStateActions)
		{
			restoreState.Restore();

			if(restoreState.DetachedHandler)
			{
				restoreState.Target->DetachedFromLogicalTree -= restoreState.DetachedHandler;
			}
		}

		_restoreStateActions.clear();
		_matchingStateStyles.clear();
	}
}