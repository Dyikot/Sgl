#include "StyleableElement.h"

#include <sstream>

namespace Sgl
{
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
		PseudoClasses(other.PseudoClasses),
		Styles(std::move(other.Styles)),
		_classList(std::move(other._classList)),
		_stylingParent(std::exchange(other._stylingParent, nullptr)),
		_isAttachedToLogicalTree(other._isAttachedToLogicalTree),
		_styles(std::move(other._styles))
	{}

	void StyleableElement::SetClasses(const std::string& classNames)
	{
		_classList.clear();

		std::stringstream stream(classNames);
		std::string buffer;

		while(stream >> buffer)
		{
			_classList.push_back(std::move(buffer));
		}

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
		for(int i = _styles.size() - 1; i >= 0; i--)
		{
			_styles[i]->Apply(*this);
		}
	}

	void StyleableElement::OnAttachedToLogicalTree()
	{
		if(FetchStyles())
		{
			ApplyStyle();
		}

		AttachedToLogicalTree(*this);
	}

	void StyleableElement::OnDetachedFromLogicalTree()
	{
		DetachedFromLogicalTree(*this);
	}

	bool StyleableElement::FetchStyles()
	{
		_styles.clear();

		FetchStylesFrom(Styles);

		if(auto parent = _stylingParent)
		{
			while(parent != nullptr)
			{
				FetchStylesFrom(parent->GetStyles());
				parent = parent->GetStylingParent();
			}
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
				_styles.push_back(&style);
			}
		}
	}

	void StyleableElement::OnStyleClassesChanged()
	{
		if(FetchStyles())
		{
			ApplyStyle();
		}
	}		
}