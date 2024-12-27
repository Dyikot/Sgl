#include "UIElement.h"

namespace Sgl
{
	UIElement::UIElement()
	{
		if(!_isStaticInitialised)
		{
			_eventInitializersMap[MouseDownEvent] = std::bind_front(InitEvent<MouseEventHandler>, MouseDownEvent);
			_eventInitializersMap[MouseUpEvent] = std::bind_front(InitEvent<MouseEventHandler>, MouseUpEvent);
			_eventInitializersMap[MouseMoveEvent] = std::bind_front(InitEvent<MouseEventHandler>, MouseMoveEvent);
			_eventInitializersMap[MouseWheelEvent] = std::bind_front(InitEvent<MouseWheelEventHandler>, MouseWheelEvent);
			_eventInitializersMap[KeyDownEvent] = std::bind_front(InitEvent<KeyEventHandler>, KeyDownEvent);
			_eventInitializersMap[KeyUpEvent] = std::bind_front(InitEvent<KeyEventHandler>, KeyUpEvent);
		}
	}

	UIElement::UIElement(const Style& style) noexcept
	{
		InitEvents(style);
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

	void UIElement::InitEvents(const Style& style)
	{
		for(auto& [id, handler] : style.EventSetters)
		{
			_eventInitializersMap[id](_events[id], handler);
		}
	}

	SetterMap<EventId> UIElement::CreateEvents()
	{
		SetterMap<EventId> events;
		events[MouseDownEvent] = Any::New<Event<MouseEventHandler>>();
		events[MouseUpEvent] = Any::New<Event<MouseEventHandler>>();
		events[MouseMoveEvent] = Any::New<Event<MouseEventHandler>>();
		events[MouseWheelEvent] = Any::New<Event<MouseWheelEventHandler>>();
		events[KeyDownEvent] = Any::New<Event<KeyEventHandler>>();
		events[KeyUpEvent] = Any::New<Event<KeyEventHandler>>();

		return events;
	}
}