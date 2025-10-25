#pragma once

#include <string>
#include <SDL/SDL_mouse.h>
#include <SDL/SDL_keyboard.h>
#include "../Base/Primitives.h"

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

	struct KeyEventArgs
	{
		ButtonState State;
		SDL_Keysym Key;
	};

	struct MouseEventArgs
	{
		FPoint Position;
	};

	struct MouseButtonEventArgs
	{
		MouseButton Button;
		ButtonState State;
		int ClicksNumber;
	};

	struct MouseWheelEventArgs
	{
		FPoint Position;
		float ScrolledHorizontally;
		float ScrolledVertically;
		MouseWheelDirection Direction;
	};
}