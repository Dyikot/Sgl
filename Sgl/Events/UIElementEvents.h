#pragma once

#include <string>
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

	struct SizeChangedEventArgs: EventArgs
	{
		int Width;
		int Height;
	};

	struct TextChangedEventArgs: EventArgs
	{
		std::string Text;
		size_t SelectionLength;
		int SelectionStart;
	};

	struct TextInputEventArgs: EventArgs
	{
		std::string Text;
	};

	class UIElement;

	using UIElementEventHandler = EventHandler<UIElement, EventArgs>;
	using MouseEventHandler = EventHandler<UIElement, MouseButtonEventArgs>;
	using MouseWheelEventHandler = EventHandler<UIElement, MouseWheelEventArgs>;
	using KeyEventHandler = EventHandler<UIElement, KeyEventArgs>;
}