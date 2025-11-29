#pragma once

#include "KeyCodes.h"
#include "KeyModifiers.h"

namespace Sgl
{	
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
		KeyCode Key;
		KeyModifier Modifier;
	};

	struct MouseMoveEventArgs
	{
		float X;
		float Y;
	};

	struct MouseButtonEventArgs
	{
		MouseButton Button;
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