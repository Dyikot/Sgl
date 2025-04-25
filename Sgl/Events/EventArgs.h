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
		ButtonState state;
		SDL_Keysym key;
	};

	enum class MouseButton
	{
		Left = 1, Middle, Right
	};

	struct MouseButtonEventArgs : EventArgs
	{
		MouseButton button;
		ButtonState state;
		uint8_t clicksCount;
		SDL_FPoint position;
	};

	struct MouseWheelEventArgs: EventArgs
	{
		SDL_FPoint position;
		float scrolledHorizontally;
		float scrolledVertically;
		SDL_MouseWheelDirection direction;
	};

	struct SizeChangedEventArgs: EventArgs
	{
		int width;
		int height;
	};

	struct TextChangedEventArgs: EventArgs
	{
		std::string text;
		size_t selectionLength;
		int selectionStart;
	};

	struct TextInputEventArgs: EventArgs
	{
		std::string text;
	};
}