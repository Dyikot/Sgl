#include "Styleable.h"
#include "../Base/Tools/StringUtils.h"
#include "../Base/Logging.h"

namespace Sgl
{
	Styleable::Styleable()
	{
		PseudoClasses.Changed += [this](PseudoClassesSet& sender, EventArgs e)
		{
			RestoreBaseState();

			if(!PseudoClasses.IsEmpty())
			{
				auto newStyles = MatchStateStyles();
				if(!newStyles.empty())
				{
					ApplyStateStyle(newStyles);
				}
			}			
		};
	}

	void Styleable::SetClasses(std::string_view classNames)
	{
		_classes = SplitString(classNames, ' ');
		FetchAndApplyStyle();
	}

	void Styleable::SetClasses(std::vector<std::string> classList)
	{
		_classes = std::move(classList);
		FetchAndApplyStyle();
	}

	const std::vector<std::string>& Styleable::GetClasses() const
	{
		return _classes;
	}

	StyleCollection& Styleable::GetStyles()
	{
		return Styles;
	}

	void Styleable::WithStyles(const Action<const StyleCollection&>& action) const
	{
		if(_stylingParent)
		{
			_stylingParent->WithStyles(action);
		}

		action(Styles);
	}

	void Styleable::SetParent(IStyleHost* parent)
	{
		_stylingParent = parent;
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
		_stateStyles.clear();

		if(!PseudoClasses.IsEmpty())
		{
			RestoreBaseState();
		}

		DetachedFromLogicalTree.Invoke(*this);
	}

	void Styleable::FetchAndApplyStylesFrom(const StyleCollection& styles)
	{
		for(auto& style : styles)
		{
			if(style.Match(*this))
			{
				if(style.HasState())
				{
					_stateStyles.push_back(&style);
				}
				else
				{
					style.Apply(*this, ValueSource::Style);
				}
			}
		}
	}

	void Styleable::FetchAndApplyStyle()
	{
		if(!IsAttachedToLogicalTree())
		{
			return;
		}

		_stateStyles.clear();
		RestoreBaseState();

		WithStyles([this](const auto& styles) { FetchAndApplyStylesFrom(styles); });

		if(!PseudoClasses.IsEmpty())
		{
			auto newStyles = MatchStateStyles();
			if(!newStyles.empty())
			{
				ApplyStateStyle(newStyles);
			}
		}		
	}

	void Styleable::ApplyStateStyle(const std::vector<const Style*>& styles)
	{
		for(auto style : styles)
		{
			style->Save(*this, _savedValues);
		}

		for(auto style : styles)
		{
			style->Apply(*this, ValueSource::PseudoClass);
		}
	}

	void Styleable::RestoreBaseState()
	{
		for(auto& savedValue : _savedValues)
		{
			savedValue->Restore();
		}

		_savedValues.clear();
	}

	std::vector<const Style*> Styleable::MatchStateStyles()
	{
		std::vector<const Style*> styles;
		styles.reserve(_stateStyles.size());

		for(auto style : _stateStyles)
		{
			if(style->MatchState(*this))
			{
				styles.push_back(style);
			}
		}

		return styles;
	}
}