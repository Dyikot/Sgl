#include "Panel.h"
#include "../Controls/Control.h"

namespace Sgl
{
	inline bool IsMouseOverControl(Control& control, SDL_FPoint mousePosition)
	{
		auto position = control.GetPosition();
		auto width = control.GetWidth();
		auto height = control.GetHeight();

		return mousePosition.x >= position.x &&
			mousePosition.x <= position.x + width &&
			mousePosition.y >= position.y &&
			mousePosition.y <= position.y + height;
	}

	Panel::Panel(const Style& style):
		Object(style)
	{}

	bool Panel::TryRaiseMouseMoveEvents(const MouseButtonEventArgs& e)
    {
		if(MouseOverControl && IsMouseOverControl(*MouseOverControl, e.Position))
		{
			MouseOverControl->OnMouseMove(e);

			if(MouseOverControl->Panel)
			{
				MouseOverControl->Panel->TryRaiseMouseMoveEvents(e);
			}

			return true;
		}

		for(Control* child : Children)
		{
			if(child == MouseOverControl)
			{
				continue;
			}

			if(IsMouseOverControl(*child, e.Position))
			{
				if(MouseOverControl)
				{
					MouseOverControl->OnMouseLeave(e);
				}

				MouseOverControl = child;
				child->OnMouseEnter(e);
				child->OnMouseMove(e);

				if(child->Panel != nullptr)
				{
					child->Panel->TryRaiseMouseMoveEvents(e);
				}

				return true;
			}
		}

		return false;
    }

    void Panel::RaiseMouseDownEvents(const MouseButtonEventArgs& e)
    {
        OnMouseDown(e);

        if(MouseOverControl)
        {
            MouseOverControl->OnMouseDown(e);
            if(MouseOverControl->Panel)
            {
                MouseOverControl->Panel->RaiseMouseDownEvents(e);
            }
        }
    }

    void Panel::RaiseMouseUpEvents(const MouseButtonEventArgs & e)
    {
        OnMouseUp(e);

        if(MouseOverControl)
        {
            MouseOverControl->OnMouseUp(e);
            if(MouseOverControl->Panel)
            {
                MouseOverControl->Panel->RaiseMouseUpEvents(e);
            }
        }
    }	
}