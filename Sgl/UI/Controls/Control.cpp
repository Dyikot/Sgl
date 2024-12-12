#include "Control.h"

namespace Sgl
{
	Control::Control(const Style& style) noexcept:
		Object(style)
	{
		style.TryInit(BackgroundProperty, _backgound);
		style.TryInitEvent(MouseDoubleClickProperty, MouseDoubleClick);
	}

	void Control::OnMouseDoubleClick(const MouseButtonEventArgs& e)
	{
		if(MouseDoubleClick) 
		{
			MouseDoubleClick(this, e);
		}
	}
}