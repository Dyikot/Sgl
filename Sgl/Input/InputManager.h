#pragma once

#include "../Base/Ref.h"
#include "MouseAndKeyEventArgs.h"

namespace Sgl
{
	class UIElement;
	class Window;

	class InputManager
	{
	public:
		void HandleMouseMove(Window& window, MouseMoveEventArgs e);
		void HandleMouseDown(Window& window, MouseClickEventArgs e);
		void HandleMouseUp(Window& window, MouseClickEventArgs e);
	private:
		Ref<UIElement> _hoveredElement;
		Ref<UIElement> _capturedElement;
	};
}