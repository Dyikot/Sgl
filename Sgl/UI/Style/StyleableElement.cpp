#include "StyleableElement.h"

namespace Sgl
{
	StyleableElement::StyleableElement():
		BindableObject(),
		Styles(),
		Classes(),
		_stylingParent(nullptr),
		_isStyleValid(false),
		_styles()
	{}

	StyleableElement::StyleableElement(const StyleableElement& other):
		BindableObject(other),
		Styles(other.Styles),
		Classes(other.Classes),
		_stylingParent(other._stylingParent),
		_isStyleValid(other._isStyleValid),
		_styles(other._styles)
	{}

	StyleableElement::StyleableElement(StyleableElement&& other) noexcept:
		BindableObject(std::move(other)),
		Styles(std::move(other.Styles)),
		Classes(std::move(other.Classes)),
		_stylingParent(std::exchange(other._stylingParent, nullptr)),
		_isStyleValid(other._isStyleValid),
		_styles(std::move(other._styles))
	{}

	void StyleableElement::ApplyStyle()
	{
		if(!_isStyleValid)
		{
			UpdateStyle();
		}

		for(int i = _styles.size() - 1; i >= 0; i--)
		{
			_styles[i]->Apply(*this);
		}
	}

	void StyleableElement::InvalidateStyle()
	{
		_isStyleValid = false;

		if(_stylingParent != nullptr)
		{
			auto styleableParent = dynamic_cast<StyleableElement*>(_stylingParent);
			if(styleableParent != nullptr)
			{
				styleableParent->InvalidateStyle();
			}
		}
	}

	void StyleableElement::UpdateStyle()
	{
		_styles.clear();

		GetStylesFrom(Styles);
		
		auto parent = _stylingParent;
		while(parent != nullptr)
		{
			GetStylesFrom(parent->GetStyles());
			parent = parent->GetStylingParent();
		}
	}

	void StyleableElement::GetStylesFrom(const StyleMap& styles)
	{
		if(styles.IsEmpty())
		{
			return;
		}

		for(auto& className : Classes)
		{
			if(auto style = styles.TryGet(className); style != nullptr)	
			{
				_styles.push_back(*style);
			}
		}
	}
}