#pragma once

#include "SDL/SDL.h"
#include <string>

namespace Sgl
{
	struct EventArgs {};

	template<typename TEventArgs>
	concept CEventArgs = std::derived_from<TEventArgs, EventArgs>;

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

	struct MouseButtonEventArgs : EventArgs
	{
		MouseButton Button;
		ButtonState State;
		uint8_t ClicksCount;
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
		std::string_view Text;
		size_t SelectionLength;
		int SelectionStart;
	};

	struct TextInputEventArgs: EventArgs
	{
		std::string_view Text;
	};
}