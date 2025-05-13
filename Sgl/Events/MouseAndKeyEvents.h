#pragma once

#include <string>
#include <SDL/SDL_mouse.h>
#include <SDL/SDL_keyboard.h>
#include "EventHandler.h"


namespace Sgl
{	
	enum class ButtonState
	{
		Release, Pressed
	};

	struct KeyEventArgs: EventArgs
	{
		ButtonState State;
		SDL_Keysym Key;
	};

	enum class MouseButton
	{
		Left = 1, Middle, Right
	};

	struct MouseButtonEventArgs: EventArgs
	{
		MouseButton Button;
		ButtonState State;
		uint8_t ClicksNumber;
		SDL_FPoint Position;
	};

	struct MouseWheelEventArgs: EventArgs
	{
		SDL_FPoint Position;
		float ScrolledHorizontally;
		float ScrolledVertically;
		SDL_MouseWheelDirection Direction;
	};

	class IKeyEventsListener
	{
	public:
		virtual ~IKeyEventsListener() = default;
	protected:
		virtual void OnKeyDown(const KeyEventArgs& e) = 0;
		virtual void OnKeyUp(const KeyEventArgs& e) = 0;
	};

	class IMouseEventsListener
	{
	public:
		virtual ~IMouseEventsListener() = default;
	protected:
		virtual void OnMouseDown(const MouseButtonEventArgs& e) = 0;
		virtual void OnMouseUp(const MouseButtonEventArgs& e) = 0;
		virtual void OnMouseMove(const MouseButtonEventArgs& e) = 0;
		virtual void OnMouseDoubleClick(const MouseButtonEventArgs& e) = 0;
		virtual void OnMouseWheel(const MouseWheelEventArgs& e) = 0;
	};
	
	using KeyEventHandler = EventHandler<IKeyEventsListener, KeyEventArgs>;
	using MouseEventHandler = EventHandler<IMouseEventsListener, MouseButtonEventArgs>;
	using MouseWheelEventHandler = EventHandler<IMouseEventsListener, MouseWheelEventArgs>;
}