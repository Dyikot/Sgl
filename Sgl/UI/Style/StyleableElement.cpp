#include "StyleableElement.h"

namespace Sgl
{
	StyleableElement::StyleableElement():
		_stylingParent(nullptr)
	{}

	void StyleableElement::ApplyStyle()
	{
		if(_shouldRestyle)
		{
			UpdateStyle();
		}

		for(int i = _styles.size() - 1; i >= 0; i--)
		{
			_styles[i]->Apply(*this);
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
				_styles.push_back(style);
			}
		}
	}
}