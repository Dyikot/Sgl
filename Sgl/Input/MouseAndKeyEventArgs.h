#pragma once

#include "KeyCodes.h"
#include "KeyModifiers.h"

namespace Sgl
{	
    /// <summary>
    /// Identifies a mouse button pressed or released during a mouse event.
    /// </summary>
    enum class MouseButton
    {
        Left = 1,
        Middle,
        Right
    };

    /// <summary>
    /// Specifies the interpretation of vertical scroll direction for mouse wheel events.
    /// </summary>
    enum class MouseWheelDirection
    {
        Normal,
        Flipped
    };

    /// <summary>
    /// Event arguments for keyboard input events, containing the pressed key and active modifiers.
    /// </summary>
    struct KeyEventArgs
    {
        KeyCode Key;
        KeyModifier Modifier;
    };

    /// <summary>
    /// Event arguments for mouse movement, providing the current cursor position in client coordinates.
    /// </summary>
    struct MouseMoveEventArgs
    {
        float X;
        float Y;
    };

    /// <summary>
    /// Event arguments for mouse button press or release events.
    /// </summary>
    struct MouseButtonEventArgs
    {
        MouseButton Button;
        int ClicksNumber;
    };

    /// <summary>
    /// Event arguments for mouse wheel (scroll) events, including scroll deltas and direction policy.
    /// </summary>
    struct MouseWheelEventArgs
    {
        float X;
        float Y;
        int ScrolledByX;
        int ScrolledByY;
        MouseWheelDirection Direction;
    };
}