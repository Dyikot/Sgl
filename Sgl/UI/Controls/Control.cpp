#include "Control.h"

namespace Sgl
{
	Control::Control():
		Control(SDL_FPoint())
	{}

	Control::Control(SDL_FPoint position) noexcept:
		Object(position)
	{}

	void Control::SetStyle(const Style& style)
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