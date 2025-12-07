#include "StyleableElement.h"

#include <sstream>
#include "../Application.h"

namespace Sgl
{
	StyleableElement::StyleableElement(const StyleableElement& other):
		AttachableObject(other),
		_classList(other._classList),
		_stylingParent(other._stylingParent),
		_isStyleApplied(other._isStyleApplied),
		_styles(other._styles)
	{}

	StyleableElement::StyleableElement(StyleableElement&& other) noexcept:
		AttachableObject(std::move(other)),
		Styles(std::move(other.Styles)),
		_classList(std::move(other._classList)),
		_stylingParent(other._stylingParent),
		_isStyleApplied(other._isStyleApplied),
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

		_isStyleApplied = true;
	}

	void StyleableElement::OnAttachedToLogicalTree(IStyleHost& parent)
	{
		SetParent(&parent);
		UpdateStyle();
		ApplyStyle();
		AttachedToElementsTree(*this);
	}

	void StyleableElement::OnDetachedFromLogicalTree()
	{
		SetParent(nullptr);
		DetachedFromElementsTree(*this);
	}

	void StyleableElement::UpdateStyle()
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
	}

	void StyleableElement::OnStyleClassesChanged()
	{
		UpdateStyle();
		ApplyStyle();
	}	

	void StyleableElement::GetStylesFrom(const StyleMap& styles)
	{
		if(styles.IsEmpty())
		{
			return;
		}

		for(auto& className : _classList)
		{
			if(auto style = styles.TryGet(className))	
			{
				_styles.push_back(style);
			}
		}
	}
}