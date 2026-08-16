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
		InputManager(Window& window);
		InputManager(const InputManager&) = delete;
		InputManager(InputManager&&) noexcept = delete;

		void HandleMouseMove(MouseMoveEventArgs e);
		void HandleMouseDown(MouseClickEventArgs& e);
		void HandleMouseUp(MouseClickEventArgs& e);
		void HandleMouseWheelChanged(MouseWheelEventArgs& e);
	private:
		Window& _window;
		Ref<UIElement> _hoveredElement;
		Ref<UIElement> _capturedElement;
	};
}