#pragma once

#include "EventHandler.h"

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

	class Window;

	using WindowStateEventHandler = EventHandler<Window, WindowStateEventArgs>;
}