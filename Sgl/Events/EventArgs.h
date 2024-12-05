#pragma once

#include "SDL/SDL.h"
#include "Event.h"

namespace Sgl
{
	struct KeyEventArgs: EventArgs
	{
		bool IsDown;
		bool IsUp;
		SDL_Keysym Key;
	};

	enum class MouseButtonState
	{
		Release, Pressed
	};

	enum class MouseButton
	{
		Left, Middle, Right
	};

	struct MouseButtonEventArgs : EventArgs
	{
		MouseButton Button;
		MouseButtonState ButtonState;
		Uint8 ClickCount;
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

	static MouseButton ToMouseButton(uint8_t sdlButton) noexcept
	{
		switch(sdlButton)
		{
			case SDL_BUTTON_LEFT: return MouseButton::Left;
			case SDL_BUTTON_RIGHT: return MouseButton::Right;
			case SDL_BUTTON_MIDDLE: return MouseButton::Middle;
		}
	}

	static MouseButtonState ToMouseButtonState(uint8_t state) noexcept
	{
		switch(state)
		{
			case SDL_PRESSED: return MouseButtonState::Pressed;
			case SDL_RELEASED: return MouseButtonState::Release;
		}
	}

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
}