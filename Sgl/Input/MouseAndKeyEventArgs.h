#pragma once

#include "KeyCodes.h"
#include "KeyModifiers.h"

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
		KeyCode Key;
		KeyModifier Modifier;
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
		float ScrolledByX;
		float ScrolledByY;
		MouseWheelDirection Direction;
	};
}