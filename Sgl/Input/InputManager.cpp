#include "InputManager.h"
#include "../Window.h"
#include "../Layout/LayoutHelper.h"

namespace Sgl
{
	struct ClearFocusHandler
	{
		FocusManager* Manager;

		void operator()(Styleable& sender, EventArgs) const
		{
			Manager->ClearFocus();
		}

		bool operator==(const ClearFocusHandler& other) const
		{
			return Manager == other.Manager;
		}
	};

	void FocusManager::SetFocus(UIElement& target)
	{
		if(!target.IsFocusable() || !target.IsAttachedToLogicalTree())
		{
			return;
		}

		ClearFocus();

		_focusedElement = &target;
		_focusedElement->DetachedFromLogicalTree += ClearFocusHandler(this);
		_focusedElement->OnGotFocus(EventArgs());
	}

	void FocusManager::ClearFocus()
	{
		if(_focusedElement)
		{
			_focusedElement->OnLostFocus(EventArgs());
			_focusedElement->DetachedFromLogicalTree -= ClearFocusHandler(this);
			_focusedElement = nullptr;
		}
	}

	UIElement* FocusManager::GetFocusedElement() const
	{
		return _focusedElement;
	}

	InputManager::InputManager(Window& window):
		_window(window)
	{}

	void InputManager::HandleMouseMove(MouseMoveEventArgs e)
	{
		FPoint point(e.X, e.Y);
		Ref<UIElement> target = _window.HitTest(point);

		SDL_SetCursor(target ? target->GetCursor() : _window.GetCursor());

		if(_hoveredElement != target && _hoveredElement && !IsPointInRect(e.X, e.Y, _hoveredElement->GetBounds()))
		{
			_hoveredElement->OnMouseMove(e);
		}

		_hoveredElement = target;

		if(_capturedElement)
		{
			_capturedElement->OnMouseMove(e);
		}
		else if(_hoveredElement)
		{
			_hoveredElement->OnMouseMove(e);
		}
	}

	void InputManager::HandleMouseDown(MouseClickEventArgs& e)
	{
		if(_hoveredElement)
		{
			_capturedElement = _hoveredElement;
			_hoveredElement->OnMouseDown(e);
			_focusManager.SetFocus(_hoveredElement.GetValue());
		}
	}

	void InputManager::HandleMouseUp(MouseClickEventArgs& e)
	{
		if(_capturedElement)
		{
			_capturedElement->OnMouseUp(e);
			_capturedElement = nullptr;
		}
	}

	void InputManager::HandleMouseWheelChanged(MouseWheelEventArgs& e)
	{
		if(_hoveredElement)
		{
			_hoveredElement->OnMouseWheelChanged(e);
		}
	}

	void InputManager::HandleKeyUp(KeyEventArgs e)
	{
		if(auto focusedElement = _focusManager.GetFocusedElement())
		{
			focusedElement->OnKeyUp(e);
		}
	}

	void InputManager::HandleKeyDown(KeyEventArgs e)
	{
		if(e.Key == KeyCodes::Escape)
		{
			_focusManager.ClearFocus();
		}
		else if(auto focusedElement = _focusManager.GetFocusedElement())
		{
			focusedElement->OnKeyDown(e);
		}
	}

	FocusManager& InputManager::GetFocusManager()
	{
		return _focusManager;
	}
}