#pragma once

#include "../Primitives.h"

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

	struct WindowStateEventArgs
	{
		WindowState State;
	};

	struct WindowVisibilityEventArgs
	{
		bool IsVisible;
	};

	struct WindowPositionChangedEventArgs
	{
		Point Position;
	};

	struct WindowSizeChangedEventArgs
	{
		size_t Width;
		size_t Height;
	};
}