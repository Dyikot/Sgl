#pragma once

#include "../Base/Ref.h"
#include "MouseAndKeyEventArgs.h"

namespace Sgl
{
	class UIElement;
	class Window;

	//! @brief Manages the keyboard focus state within a window, tracking the currently focused UI element
	class FocusManager
	{
	public:
		//! @brief Constructs a focus manager for the specified window
		//! @param window The window that this focus manager operates on
		FocusManager(Window& window);

		FocusManager(const FocusManager&) = delete;
		FocusManager(FocusManager&&) = delete;

		//! @brief Sets the keyboard focus to the specified UI element
		//! @param target The UI element to receive focus
		//! @return True if the focus was successfully set; otherwise, false
		bool SetFocus(Ref<UIElement> target);

		//! @brief Moves the keyboard focus to the next focusable element
		//! @return True if the focus was successfully moved; otherwise, false
		bool MoveFocusNext();

		//! @brief Clears the keyboard focus from the currently focused element
		void ClearFocus();

		//! @brief Gets the UI element that currently has keyboard focus
		//! @return A reference to the focused element, or an empty reference if no element is focused
		Ref<UIElement> GetFocusedElement() const;

	private:
		bool FocusFirst(const Ref<UIElement>& element);
		bool FocusNext(const Ref<UIElement>& element);

	private:
		Ref<UIElement> _focusedElement;
		Window& _window;
	};

	//! @brief Handles input events such as mouse movements, clicks, and keyboard presses, routing them to the appropriate UI elements
	class InputManager
	{
	public:
		//! @brief Constructs an input manager for the specified window
		//! @param window The window that this input manager operates on
		InputManager(Window& window);

		InputManager(const InputManager&) = delete;
		InputManager(InputManager&&) = delete;

		//! @brief Handles a mouse movement event
		//! @param e The mouse move event arguments
		void HandleMouseMove(MouseMoveEventArgs e);

		//! @brief Handles a mouse button press event
		//! @param e The mouse click event arguments
		void HandleMouseDown(MouseClickEventArgs& e);

		//! @brief Handles a mouse button release event
		//! @param e The mouse click event arguments
		void HandleMouseUp(MouseClickEventArgs& e);

		//! @brief Handles a mouse wheel scroll event
		//! @param e The mouse wheel event arguments
		void HandleMouseWheelChanged(MouseWheelEventArgs& e);

		//! @brief Handles a keyboard key release event
		//! @param e The key event arguments
		void HandleKeyUp(KeyEventArgs e);

		//! @brief Handles a keyboard key press event
		//! @param e The key event arguments
		void HandleKeyDown(KeyEventArgs e);

		//! @brief Gets the focus manager associated with this input manager
		//! @return A reference to the focus manager
		FocusManager& GetFocusManager();

	private:
		Window& _window;
		Ref<UIElement> _hoveredElement;
		Ref<UIElement> _capturedElement;
		FocusManager _focusManager;
	};
}