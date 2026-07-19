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
		void HandleMouseDown(Window& window, MouseButtonEventArgs e);
		void HandleMouseUp(Window& window, MouseButtonEventArgs e);
	private:
		Ref<UIElement> _hoveredElement;
		Ref<UIElement> _capturedElement;
	};
}