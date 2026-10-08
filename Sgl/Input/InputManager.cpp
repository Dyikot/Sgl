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

	const Ref<UIElement>& FocusManager::GetFocusedElement() const
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
			HandleMouseMove(e, _hoveredElement.Get());
		}

		_hoveredElement = target;

		if(_capturedElement)
		{
			HandleMouseMove(e, _capturedElement.Get());
		}
		else if(_hoveredElement)
		{
			HandleMouseMove(e, _hoveredElement.Get());
		}
	}

	void InputManager::HandleMouseDown(MouseClickEventArgs& e)
	{
		if(_hoveredElement)
		{
			_capturedElement = _hoveredElement;
			HandleMouseDown(e, _hoveredElement.Get());
			_focusManager.SetFocus(FindFocusableAncestor(_hoveredElement));
		}
	}

	void InputManager::HandleMouseUp(MouseClickEventArgs& e)
	{
		HandleMouseUp(e, _capturedElement.Get());
		_capturedElement = nullptr;
	}

	void InputManager::HandleMouseWheelChanged(MouseWheelEventArgs& e)
	{
		UIElement* current = _hoveredElement.Get();
		while(current && !e.Handled)
		{
			current->OnMouseWheelChanged(e);
			current = current->GetParent();
		}
	}

	void InputManager::HandleKeyUp(KeyEventArgs& e)
	{
		UIElement* current = _focusManager.GetFocusedElement().Get();
		while(current && !e.Handled)
		{
			current->OnKeyUp(e);
			current = current->GetParent();
		}
	}

	void InputManager::HandleKeyDown(KeyEventArgs& e)
	{
		if(e.Key == KeyCodes::Escape)
		{
			_focusManager.ClearFocus();
		}
		else if(e.Key == KeyCodes::Tab && e.Modifier == KeyModifiers::None)
		{
			_focusManager.MoveFocusNext();
		}
		else
		{
			UIElement* current = _focusManager.GetFocusedElement().Get();
			while(current && !e.Handled)
			{
				current->OnKeyDown(e);
				current = current->GetParent();
			}
		}
	}

	FocusManager& InputManager::GetFocusManager()
	{
		return _focusManager;
	}

	void InputManager::HandleMouseMove(MouseMoveEventArgs e, UIElement* current)
	{
		while(current)
		{
			current->OnMouseMove(e);
			current = current->GetParent();
		}
	}

	void InputManager::HandleMouseDown(MouseClickEventArgs& e, UIElement* current)
	{
		while(current && !e.Handled)
		{
			current->OnMouseDown(e);
			current = current->GetParent();
		}
	}

	void InputManager::HandleMouseUp(MouseClickEventArgs& e, UIElement* current)
	{
		while(current && !e.Handled)
		{
			current->OnMouseUp(e);
			current = current->GetParent();
		}
	}

	Ref<UIElement> InputManager::FindFocusableAncestor(const Ref<UIElement>& element)
	{
		UIElement* current = element.Get();

		while(current)
		{
			if(current->IsFocusable())
			{
				return Ref(current);
			}

			current = current->GetParent();
		}

		return nullptr;
	}
}