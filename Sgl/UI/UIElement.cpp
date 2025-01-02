#include "UIElement.h"

namespace Sgl
{
	UIElement::UIElement():
		_isEventsInitialized(InitializeEvents()),
		MouseDown(GetEventValue<MouseEventHandler>(MouseDownEvent)),
		MouseUp(GetEventValue<MouseEventHandler>(MouseUpEvent)),
		MouseMove(GetEventValue<MouseEventHandler>(MouseMoveEvent)),
		MouseWheel(GetEventValue<MouseWheelEventHandler>(MouseWheelEvent)),
		KeyDown(GetEventValue<KeyEventHandler>(KeyDownEvent)),
		KeyUp(GetEventValue<KeyEventHandler>(KeyUpEvent))
	{}

	void UIElement::SetStyle(const Style& style)
	{
		for(auto& [id, eventHanlder] : style.EventSetters)
		{
			SetEvent(id, eventHanlder);
		}
	}

	void UIElement::OnMouseDown(const MouseButtonEventArgs& e)
	{
		if(MouseDown)
		{
			MouseDown(this, e);
		}
	}

	void UIElement::OnMouseUp(const MouseButtonEventArgs& e)
	{
		if(MouseUp)
		{
			MouseUp(this, e);
		}
	}

	void UIElement::OnMouseMove(const MouseButtonEventArgs& e)
	{
		if(MouseMove)
		{
			MouseMove(this, e);
		}
	}

	void UIElement::OnMouseWheel(const MouseWheelEventArgs& e)
	{
		if(MouseWheel)
		{
			MouseWheel(this, e);
		}
	}

	void UIElement::OnKeyDown(const KeyEventArgs& e)
	{
		if(KeyDown)
		{
			KeyDown(this, e);
		}
	}

	void UIElement::OnKeyUp(const KeyEventArgs& e)
	{
		if(KeyUp)
		{
			KeyUp(this, e);
		}
	}

	bool UIElement::InitializeEvents()
	{
		AddEvent<MouseEventHandler>(MouseDownEvent);
		AddEvent<MouseEventHandler>(MouseUpEvent);
		AddEvent<MouseEventHandler>(MouseMoveEvent);
		AddEvent<MouseWheelEventHandler>(MouseWheelEvent);
		AddEvent<KeyEventHandler>(KeyDownEvent);
		AddEvent<KeyEventHandler>(KeyUpEvent);

		return true;
	}
}