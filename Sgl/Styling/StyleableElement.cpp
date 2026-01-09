#include "StyleableElement.h"

#include <sstream>
#include "../Application.h"

namespace Sgl
{
	StyleableElement::StyleableElement(const StyleableElement& other):
		BindableObject(other),
		_classList(other._classList),
		_stylingParent(other._stylingParent),
		_stylingRoot(other._stylingRoot),
		_styles(other._styles)
	{}

	StyleableElement::StyleableElement(StyleableElement&& other) noexcept:
		BindableObject(std::move(other)),
		Styles(std::move(other.Styles)),
		_classList(std::move(other._classList)),
		_stylingParent(std::exchange(other._stylingParent, nullptr)),
		_stylingRoot(std::exchange(other._stylingRoot, nullptr)),
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
		if(parent == nullptr)
		{
			_stylingParent = nullptr;
			SetStylingRoot(nullptr);
			return;
		}

		_stylingParent = parent;

		if(auto stylingRoot = parent->GetStylingRoot())
		{
			SetStylingRoot(stylingRoot);
		}
	}

	void StyleableElement::SetStylingRoot(IStyleHost* value)
	{
		_stylingRoot = value;
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
		if(UpdateStyle())
		{
			ApplyStyle();
		}

		AttachedToElementsTree(*this);
	}

	void StyleableElement::OnDetachedFromLogicalTree()
	{
		DetachedFromElementsTree(*this);
	}

	bool StyleableElement::UpdateStyle()
	{
		_styles.clear();

		GetStylesFrom(Styles);

		if(auto parent = _stylingParent)
		{
			while(parent != nullptr)
			{
				GetStylesFrom(parent->GetStyles());
				parent = parent->GetStylingParent();
			}
		}

		return _styles.size() > 0;
	}

	void StyleableElement::OnStyleClassesChanged()
	{
		if(UpdateStyle())
		{
			ApplyStyle();
		}
	}	

	void StyleableElement::GetStylesFrom(const StyleCollection& styles)
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
}