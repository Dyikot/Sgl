#include "Control.h"

void Sgl::Control::SetStyle(const Style& style)
{
	Object::SetStyle(style);
	//style.TryInit(Back)
}

void Sgl::Control::OnMouseDoubleClick(const MouseButtonEventArgs& e)
{
	if(MouseDoubleClick)
	{
		MouseDoubleClick(this, e);
	}
}
