#include "InputManager.h"
#include "../Window.h"
#include "../Layout/LayoutHelper.h"

namespace Sgl
{
	void InputManager::HandleMouseMove(Window& window, MouseMoveEventArgs e)
	{
		FPoint point(e.X, e.Y);
		Ref<UIElement> target = window.HitTest(point);

		SDL_SetCursor(target ? target->GetCursor() : window.GetCursor());

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

	void InputManager::HandleMouseDown(Window& window, MouseClickEventArgs e)
	{
		if(_hoveredElement)
		{
			_capturedElement = _hoveredElement;
			_hoveredElement->OnMouseDown(e);
		}
	}

	void InputManager::HandleMouseUp(Window& window, MouseClickEventArgs e)
	{
		if(_capturedElement)
		{
			_capturedElement->OnMouseUp(e);
			_capturedElement = nullptr;
		}
	}
}