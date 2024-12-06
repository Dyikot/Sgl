#include "Control.h"

namespace Sgl
{
	Control::Control(const Style& style) noexcept:
		Object(style)
	{
		style.TryCopyTo(BackgroundProperty, _backgound);
		style.TryAddHandlerToEvent(MouseDoubleClickProperty, MouseDoubleClick);
	}

	void Control::OnMouseDoubleClick(const MouseButtonEventArgs& e)
	{
		if(MouseDoubleClick) 
		{
			MouseDoubleClick(this, e);
		}
	}
}