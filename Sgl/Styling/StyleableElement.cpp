#include "StyleableElement.h"
#include "../Base/Tools/StringUtils.h"
#include "../Base/Logging.h"

namespace Sgl
{
	StyleableElement::StyleableElement()
	{
		PseudoClasses.Changed += [this](PseudoClassesSet& sender, EventArgs e)
		{
			RestoreBaseState();

			if(PseudoClasses.IsEmpty())
			{
				_restoreStateActions.clear();
				return;
			}

			SaveBaseState();
			ApplyStateStyle();			
		};
	}

	StyleableElement::StyleableElement(const StyleableElement& other):
		BindableObject(other),
		PseudoClasses(other.PseudoClasses),
		_classList(other._classList),
		_stylingParent(other._stylingParent),
		_isAttachedToLogicalTree(false),
		_styles(other._styles)
	{}

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

		if(FetchStyles())
		{
			ApplyStyle();
			
			if(!PseudoClasses.IsEmpty())
			{
				SaveBaseState();
				ApplyStateStyle();
			}
		}

		AttachedToLogicalTree(*this);
	}

	void StyleableElement::OnDetachedFromLogicalTree()
	{
		_isAttachedToLogicalTree = false;
		ClearRestoreActionsTargeting(this);
		DetachedFromLogicalTree(*this);
	}

	void StyleableElement::ApplyStateStyle()
	{
		for(auto style : _stateStyles)
		{
			if(style->Selector.MatchState(*this))
			{
				style->Apply(*this, ValueSource::PseudoClass);
			}
		}
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

		return _styles.size() > 0;
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

	void StyleableElement::SaveBaseState()
	{
		if(!_restoreStateActions.empty() || _stateStyles.empty())
		{
			return;
		}

		for(auto style : _stateStyles)
		{
			auto& target = style->Projection ? style->Projection(*this) : *this;
			for(auto& setter : style->_setters)
			{
				auto& property = setter->GetProperty();
				_restoreStateActions.emplace_back(
					property.CreateRestoreAction(&target),
					&target
				);
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

	void StyleableElement::ClearRestoreActionsTargeting(StyleableElement* target)
	{
		auto byTarget = [target](const RestoreAction& action)
		{
			return action.Target == target;
		};

		std::erase_if(_restoreStateActions, byTarget);

		IStyleHost* parent = GetStylingParent();
		while(parent != nullptr)
		{
			if(auto* element = dynamic_cast<StyleableElement*>(parent))
			{
				std::erase_if(element->_restoreStateActions, byTarget);
			}

			parent = parent->GetStylingParent();
		}
	}
}