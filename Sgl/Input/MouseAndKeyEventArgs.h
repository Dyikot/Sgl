#pragma once

#include <string>
#include <SDL3/SDL_keyboard.h>

namespace Sgl
{	
	enum class ButtonState
	{
		Released, Pressed
	};	

	enum class MouseButton
	{
		Left = 1, Middle, Right
	};

	enum class MouseWheelDirection
	{
		Normal, Flipped
	};

	struct KeyEventArgs
	{
		ButtonState State;
		SDL_Keycode Key;
		SDL_Keymod Modifier;
	};

	struct MouseEventArgs
	{
		float X;
		float Y;
	};

	struct MouseButtonEventArgs
	{
		MouseButton Button;
		ButtonState State;
		int ClicksNumber;
	};

	struct MouseWheelEventArgs
	{
		float X;
		float Y;
		float ScrolledHorizontally;
		float ScrolledVertically;
		MouseWheelDirection Direction;
	};
}