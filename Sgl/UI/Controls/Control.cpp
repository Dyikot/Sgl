#include "Control.h"

namespace Sgl
{
	Control::Control(const Style& style) noexcept:
		Object(style)
	{
		/*style.TryInit(BackgroundProperty, _backgound);
		style.TryInit(BorderColorProperty, _borderColor);
		style.TryInit(BorderThiknessProperty, _borderThickness);
		style.TryInit(FontFamilyProperty, _fontFamily);
		style.TryInit(FontWeightProperty, _fontWeight);
		style.TryInit(FontColorProperty, _fontColor);
		style.TryInit(FontSizeProperty, _fontSize);
		style.TryInitEvent(MouseDoubleClickProperty, MouseDoubleClick);*/
	}

	void Control::OnMouseDoubleClick(const MouseButtonEventArgs& e)
	{
		if(MouseDoubleClick) 
		{
			MouseDoubleClick(this, e);
		}
	}
}