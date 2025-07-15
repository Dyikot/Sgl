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
}