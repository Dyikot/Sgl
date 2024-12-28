#include "Control.h"

namespace Sgl
{
	Control::Control(const Style& style, SDL_FPoint position) noexcept:
		Object(style, position)
	{
		style.EventSetters.TrySetEvent(MouseDoubleClickEvent, MouseDoubleClick);
	}

	void Control::OnMouseDoubleClick(const MouseButtonEventArgs& e)
	{
		if(MouseDoubleClick) 
		{
			MouseDoubleClick(this, e);
		}
	}
}