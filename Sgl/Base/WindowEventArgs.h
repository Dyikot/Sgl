#pragma once

#include "EventArgs.h"

namespace Sgl
{
	enum class DisplayMode
	{
		Window, BorderlessWindow, Fullscreen
	};

	enum class WindowState
	{
		Normal, Minimized, Maximized
	};

	struct WindowStateEventArgs: EventArgs
	{
		WindowState State;
	};

	struct WindowVisibilityEventArgs: EventArgs
	{
		bool IsVisible;
	};

	struct WindowPositionChangedEventArgs: EventArgs
	{
		Point Position;
	};

	struct WindowSizeChangedEventArgs: EventArgs
	{
		size_t Width;
		size_t Height;
	};
}