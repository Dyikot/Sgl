#pragma once

#include <string>
#include <SDL/SDL_mouse.h>
#include <SDL/SDL_keyboard.h>
#include "EventHandler.h"
#include "../Visual/Primitives.h"


namespace Sgl
{	
	enum class ButtonState
	{
		Release, Pressed
	};	

	enum class MouseButton
	{
		Left = 1, Middle, Right
	};

	enum class MouseWheelDirection
	{
		Normal, Flipped
	};

	struct KeyEventArgs: EventArgs
	{
		ButtonState State;
		SDL_Keysym Key;
	};

	struct MouseButtonEventArgs: EventArgs
	{
		FPoint Position;
		MouseButton Button;
		ButtonState State;
		int ClicksNumber;
	};

	struct MouseWheelEventArgs: EventArgs
	{
		FPoint Position;
		float ScrolledHorizontally;
		float ScrolledVertically;
		MouseWheelDirection Direction;
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
		virtual void OnMouseWheel(const MouseWheelEventArgs& e) = 0;
	};
}