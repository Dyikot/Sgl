#pragma once

#include "KeyCodes.h"
#include "KeyModifiers.h"

namespace Sgl
{
	//! @brief Identifies a mouse button pressed or released during a mouse event
	enum class MouseButton
	{
		Left,
		Middle,
		Right,
		XButton1,
		XButton2
	};

	//! @brief Specifies the interpretation of vertical scroll direction for mouse wheel events
	enum class MouseWheelDirection
	{
		Normal,
		Flipped
	};

	//! @brief Event arguments for keyboard input events, containing the pressed key and active modifiers
	struct KeyEventArgs
	{
		KeyCode Key;
		KeyModifier Modifier;
	};

	//! @brief Event arguments for mouse movement, providing the current cursor position in client coordinates
	struct MouseMoveEventArgs
	{
		float X;
		float Y;
	};

	//! @brief Event arguments for mouse button press or release events
	struct MouseClickEventArgs : MouseMoveEventArgs
	{
		MouseButton Button;
		int ClicksNumber;
	};

	//! @brief Event arguments for mouse wheel (scroll) events, including scroll deltas and direction policy
	struct MouseWheelEventArgs : MouseMoveEventArgs
	{
		int ScrolledByX;
		int ScrolledByY;
		MouseWheelDirection Direction;
	};
}