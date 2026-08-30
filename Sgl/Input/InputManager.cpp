#include "InputManager.h"
#include "../Window.h"
#include "../Layout/LayoutHelper.h"

#include <span>

namespace Sgl
{
	FocusManager::FocusManager(Window& window):
		_window(window)
	{}

	bool FocusManager::SetFocus(Ref<UIElement> target)
	{
		if(!target || !target->IsFocusable() || !target->IsAttachedToLogicalTree())
		{
			return false;
		}

		if(_focusedElement)
		{
			_focusedElement->OnLostFocus(EventArgs());
		}

		_focusedElement = std::move(target);
		_focusedElement->OnGotFocus(EventArgs());
		return true;
	}

	bool FocusManager::MoveFocusNext()
	{
		if(_focusedElement)
		{
			return FocusFirst(_focusedElement) || FocusNext(_focusedElement);
		}

		if(auto& first = _window.GetContent())
		{
			return SetFocus(first) || FocusFirst(first);
		}

		return false;
	}

	void FocusManager::ClearFocus()
	{
		if(_focusedElement)
		{
			_focusedElement->OnLostFocus(EventArgs());
			_focusedElement = nullptr;
		}
	}

	Ref<UIElement> FocusManager::GetFocusedElement() const
	{
		return _focusedElement;
	}

	bool FocusManager::FocusFirst(const Ref<UIElement>& element)
	{
		for(auto& child : element->GetChildren())
		{
			if(SetFocus(child) || FocusFirst(child))
			{
				return true;
			}
		}

		return false;
	}

	bool FocusManager::FocusNext(const Ref<UIElement>& element)
	{
		auto parent = Ref(element->GetParent());

		if(!parent)
		{
			if(auto& first = _window.GetContent())
			{
				return SetFocus(first) || FocusFirst(first);
			}

			return false;
		}

		auto& children = parent->GetChildren();			

		if(auto it = std::ranges::find(children, element); it != children.end())
		{
			for(auto& child : std::span(std::ranges::next(it), children.end()))
			{
				if(SetFocus(child) || FocusFirst(child))
				{
					return true;
				}
			}
		}

		return FocusNext(parent);
	}

	InputManager::InputManager(Window& window):
		_window(window),
		_focusManager(window)
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
			_focusManager.SetFocus(_hoveredElement);
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
		else if(e.Key == KeyCodes::Tab && e.Modifier == KeyModifiers::None)
		{
			_focusManager.MoveFocusNext();
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