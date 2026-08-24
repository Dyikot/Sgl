#pragma once

#include "../Base/Ref.h"
#include "MouseAndKeyEventArgs.h"

namespace Sgl
{
	class UIElement;
	class Window;

	class FocusManager
	{
	public:
		FocusManager() = default;
		FocusManager(const FocusManager&) = delete;
		FocusManager(FocusManager&&) = delete;

		void SetFocus(UIElement& target);
		void ClearFocus();
		UIElement* GetFocusedElement() const;
	private:
		UIElement* _focusedElement {};
	};

	class InputManager
	{
	public:
		InputManager(Window& window);
		InputManager(const InputManager&) = delete;
		InputManager(InputManager&&) = delete;

		void HandleMouseMove(MouseMoveEventArgs e);
		void HandleMouseDown(MouseClickEventArgs& e);
		void HandleMouseUp(MouseClickEventArgs& e);
		void HandleMouseWheelChanged(MouseWheelEventArgs& e);
		void HandleKeyUp(KeyEventArgs e);
		void HandleKeyDown(KeyEventArgs e);

		FocusManager& GetFocusManager();
	private:
		Window& _window;
		Ref<UIElement> _hoveredElement;
		Ref<UIElement> _capturedElement;
		FocusManager _focusManager;
	};
}