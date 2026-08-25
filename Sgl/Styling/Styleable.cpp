#include "Styleable.h"
#include "../Base/Tools/StringUtils.h"
#include "../Base/Logging.h"

namespace Sgl
{
	Styleable::Styleable()
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

	Styleable::Styleable(Styleable&& other) noexcept:
		Bindable(std::move(other)),
		Name(std::move(other.Name)),
		PseudoClasses(std::move(other.PseudoClasses)),
		Styles(std::move(other.Styles)),
		_classList(std::move(other._classList)),
		_stylingParent(std::exchange(other._stylingParent, nullptr)),
		_isAttachedToLogicalTree(other._isAttachedToLogicalTree),
		_styles(std::move(other._styles))
	{}

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

	void Styleable::OnDetachedFromLogicalTree()
	{
		_isAttachedToLogicalTree = false;

		if(!PseudoClasses.IsEmpty())
		{
			RestoreBaseState();
			ClearMatchingStateStyles();
		}

		DetachedFromLogicalTree.Invoke(*this);
	}

	bool Styleable::FetchStyles()
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

	void Styleable::FetchStylesFrom(const StyleCollection& styles)
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

	void Styleable::OnStyleClassesChanged()
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

	void Styleable::ApplyStateStyle()
	{
		for(auto style : _matchingStateStyles)
		{
			style->Apply(*this, ValueSource::PseudoClass);
		}
	}

	void Styleable::SaveBaseState()
	{
		for(auto style : _matchingStateStyles)
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

	void Styleable::ClearMatchingStateStyles()
	{
		_matchingStateStyles.clear();
	}

	bool Styleable::MatchStateStyles()
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
}