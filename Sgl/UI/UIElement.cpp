#include "UIElement.h"

namespace Sgl::UI
{
	UIElement::UIElement():
		Class(*this),
		OnHover(Class),
		OnMousePressed(Class),
		_width(),
		_height(),
		_minWidth(),
		_minHeight(),
		_maxWidth(std::numeric_limits<float>::max()),
		_maxHeight(std::numeric_limits<float>::max()),
		_zIndex(1),
		_tooltip(),
		_visibility(Visibility::Visible),
		_verticalAlignment(VerticalAlignment::Top),
		_horizontalAlignment(HorizontalAlignment::Left),
		_margin(),
		_actualWidth(_width - _margin.Right),
		_actualHeight(_height - _margin.Bottom),
		_position(),
		_actualPosition(_position.x + _margin.Left, _position.y + _margin.Right)
	{}

	UIElement::UIElement(const UIElement& other):
		base(other),
		Class(other.Class),
		OnHover(other.OnHover),
		OnMousePressed(other.OnMousePressed),
		_width(other._width),
		_height(other._height),
		_minWidth(other._minWidth),
		_minHeight(other._minHeight),
		_maxWidth(other._maxWidth),
		_maxHeight(other._maxHeight),
		_zIndex(other._zIndex),
		_tooltip(other._tooltip),
		_visibility(other._visibility),
		_verticalAlignment(other._verticalAlignment),
		_horizontalAlignment(other._horizontalAlignment),
		_margin(other._margin),
		_actualWidth(other._actualWidth),
		_actualHeight(other._actualHeight),
		_position(other._position),
		_actualPosition(other._actualPosition)
	{}

	UIElement::UIElement(UIElement&& other) noexcept:
		base(std::move(other)),
		Class(std::move(other.Class)),
		OnHover(std::move(other.OnHover)),
		OnMousePressed(std::move(other.OnMousePressed)),
		_width(other._width),
		_height(other._height),
		_minWidth(other._minWidth),
		_minHeight(other._minHeight),
		_maxWidth(other._maxWidth),
		_maxHeight(other._maxHeight),
		_zIndex(other._zIndex),
		_tooltip(std::move(other._tooltip)),
		_visibility(other._visibility),
		_verticalAlignment(other._verticalAlignment),
		_horizontalAlignment(other._horizontalAlignment),
		_margin(other._margin),
		_actualWidth(other._actualWidth),
		_actualHeight(other._actualHeight),
		_position(other._position),
		_actualPosition(other._actualPosition)
	{}

	void UIElement::OnRender(RenderContext rc) const
	{		
		if(OnHover.IsActive() && _tooltip)
		{
			_tooltip->OnRender(rc);
		}
	}

	void UIElement::ResetToDefault()
	{
		base::ResetToDefault();

		SetWidth(0);
		SetHeight(0);
		_minWidth = 0;
		_minHeight = 0;
		_maxWidth = std::numeric_limits<float>::max();
		_maxHeight = std::numeric_limits<float>::max();
		_zIndex = 1;
		_tooltip = {};
		_visibility = Visibility::Visible;
		_verticalAlignment = VerticalAlignment::Top;
		_horizontalAlignment = HorizontalAlignment::Left;
		_margin = {};
	}

	void UIElement::OnMouseDown(const MouseButtonEventArgs& e)
	{
		OnMousePressed.Activate();
	}

	void UIElement::OnMouseUp(const MouseButtonEventArgs& e)
	{
		OnMousePressed.Deactivate();
	}

	void UIElement::OnMouseEnter(const MouseButtonEventArgs& e)
	{
		OnHover.Activate();
	}

	void UIElement::OnMouseLeave(const MouseButtonEventArgs& e)
	{
		OnHover.Deactivate();
	}

	void UIElement::OnSizeChanged()
	{
		SizeChanged.TryRaise(*this);
	}
}