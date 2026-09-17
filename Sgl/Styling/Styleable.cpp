#include "Styleable.h"
#include "../Base/Tools/StringUtils.h"
#include "../Base/Logging.h"

namespace Sgl
{
	Styleable::Styleable()
	{
		PseudoClasses.Changed += [this](PseudoClassesSet& sender, EventArgs e)
		{
			auto previousStyleStates = std::move(_activeStateStyles);
			bool match = MatchStateStyles();

			if(previousStyleStates == _activeStateStyles)
			{
				return;
			}

			RestoreBaseState();

			if(match)
			{
				SaveBaseState();
				ApplyStateStyle();
			}
		};
	}

	void Styleable::SetClasses(std::string_view classNames)
	{
		_classList = SplitString(classNames, ' ');
		OnStyleClassesChanged();
	}

	void Styleable::SetClasses(std::vector<std::string> classList)
	{
		_classList = std::move(classList);
		OnStyleClassesChanged();
	}

	const std::vector<std::string>& Styleable::GetClasses() const
	{
		return _classList;
	}

	StyleCollection& Styleable::GetStyles()
	{
		return Styles;
	}

	std::vector<const StyleCollection*> Styleable::GetAllStyles() const
	{
		if(_stylingParent)
		{
			auto styles = _stylingParent->GetAllStyles();
			styles.push_back(&Styles);
			return styles;
		}

		return { &Styles };
	}

	void Styleable::SetParent(IStyleHost* parent)
	{
		_stylingParent = parent;
	}

	void Styleable::ApplyStyle()
	{
		for(auto style : _styles)
		{
			style->Apply(*this, ValueSource::Style);
		}
	}

	void Styleable::OnAttachedToLogicalTree()
	{
		_isAttachedToLogicalTree = true;
		AttachedToLogicalTree.Invoke(*this);
		FetchAndApplyStyle();
	}

	void Styleable::OnDetachedFromLogicalTree()
	{
		_isAttachedToLogicalTree = false;

		if(!PseudoClasses.IsEmpty())
		{
			RestoreBaseState();
			_activeStateStyles.clear();
		}

		_styles.clear();
		_stateStyles.clear();

		DetachedFromLogicalTree.Invoke(*this);
	}

	bool Styleable::FetchStyles()
	{
		_styles.clear();
		_stateStyles.clear();

		auto styleCollections = GetAllStyles();

		for(auto it = styleCollections.rbegin(); it != styleCollections.rend(); ++it)
		{
			FetchStylesFrom(**it);
		}

		return !_styles.empty() || !_stateStyles.empty();
	}

	void Styleable::FetchAndApplyStyle()
	{
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

	void Styleable::FetchStylesFrom(const StyleCollection& styles)
	{
		for(auto& style : styles)
		{
			auto& selector = style.GetSelector();
			
			if(selector.Match(*this))
			{
				if(selector.HasState())
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

	void Styleable::OnStyleClassesChanged()
	{
		if(!IsAttachedToLogicalTree())
		{
			return;
		}

		_activeStateStyles.clear();
		RestoreBaseState();
		FetchAndApplyStyle();
	}

	void Styleable::ApplyStateStyle()
	{
		for(auto style : _activeStateStyles)
		{
			style->Apply(*this, ValueSource::PseudoClass);
		}
	}

	void Styleable::SaveBaseState()
	{
		for(auto style : _activeStateStyles)
		{
			auto& target = style->SelectTarget(*this);

			for(auto& setter : style->_setters)
			{
				auto& property = setter->GetProperty();
				auto stateGuard = property.CreateStateGuard(target);
				_propertyGuards.emplace_back(stateGuard);
			}
		}
	}

	void Styleable::RestoreBaseState()
	{
		_propertyGuards.clear();
	}

	bool Styleable::MatchStateStyles()
	{
		_activeStateStyles.clear();

		for(auto style : _stateStyles)
		{
			if(style->GetSelector().MatchState(*this))
			{
				_activeStateStyles.push_back(style);
			}
		}

		return !_activeStateStyles.empty();
	}
}