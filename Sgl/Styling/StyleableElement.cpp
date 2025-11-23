#include "StyleableElement.h"
#include <sstream>
#include "../Application.h"

namespace Sgl
{
	StyleableElement::StyleableElement(const StyleableElement& other):
		AttachableObject(other),
		_classList(other._classList),
		_styleableParent(other._styleableParent),
		_isStyleValid(other._isStyleValid),
		_styles(other._styles)
	{}

	StyleableElement::StyleableElement(StyleableElement&& other) noexcept:
		AttachableObject(std::move(other)),
		Styles(std::move(other.Styles)),
		_classList(std::move(other._classList)),
		_styleableParent(std::exchange(other._styleableParent, nullptr)),
		_isStyleValid(other._isStyleValid),
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

		InvalidateStyle();
	}

	void StyleableElement::SetClasses(std::vector<std::string> classList)
	{
		_classList = std::move(classList);
		InvalidateStyle();
	}

	const std::vector<std::string>& StyleableElement::GetClasses() const
	{
		return _classList;
	}

	void StyleableElement::SetParent(StyleableElement* parent)
	{
		_styleableParent = parent;
	}

	void StyleableElement::ApplyStyle()
	{
		_isStyleValid = true;
		UpdateStyles();

		for(int i = _styles.size() - 1; i >= 0; i--)
		{
			_styles[i]->Apply(*this);
		}
	}

	void StyleableElement::InvalidateStyle()
	{
		if(_isStyleValid)
		{
			_isStyleValid = false;

			if(_styleableParent)
			{
				_styleableParent->InvalidateStyle();				
			}			
		}
	}

	void StyleableElement::UpdateStyles()
	{
		_styles.clear();

		GetStylesFrom(Styles);
		
		auto parent = _styleableParent;
		while(parent != nullptr)
		{
			GetStylesFrom(parent->Styles);
			parent = parent->GetStyleableParent();
		}

		GetStylesFrom(App->Styles);
	}

	void StyleableElement::GetStylesFrom(const StyleMap& styles)
	{
		if(styles.IsEmpty())
		{
			return;
		}

		for(auto& className : _classList)
		{
			if(auto style = styles.TryGet(className); style != nullptr)	
			{
				_styles.push_back(style);
			}
		}
	}
}