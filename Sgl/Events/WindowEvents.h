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

	class Window;

	struct WindowStateEventArgs: EventArgs
	{
		WindowState State;
	};

	using WindowStateEventHandler = EventHandler<Window, WindowStateEventArgs>;
}