#include "UIElement.h"

namespace Sgl
{
	UIElement::UIElement():
		_cursor(Cursors::Arrow),
		_backgroundColor(Colors::Black),
		_backgroundTexture(),
		_width(),
		_height(),
		_minWidth(),
		_minHeight(),
		_maxWidth(std::numeric_limits<float>::max()),
		_maxHeight(std::numeric_limits<float>::max()),
		_zIndex(1),
		_isVisible(true),
		_verticalAlignment(VerticalAlignment::Top),
		_horizontalAlignment(HorizontalAlignment::Left),
		_margin(),
		_actualWidth(_width - _margin.Right),
		_actualHeight(_height - _margin.Bottom),
		_position(),
		_actualPosition(_position.x + _margin.Left, _position.y + _margin.Right)
	{}

	UIElement::UIElement(const UIElement& other):
		_cursor(other._cursor),
		_backgroundColor(other._backgroundColor),
		_backgroundTexture(other._backgroundTexture),
		_width(other._width),
		_height(other._height),
		_minWidth(other._minWidth),
		_minHeight(other._minHeight),
		_maxWidth(other._maxWidth),
		_maxHeight(other._maxHeight),
		_zIndex(other._zIndex),
		_isVisible(other._isVisible),
		_verticalAlignment(other._verticalAlignment),
		_horizontalAlignment(other._horizontalAlignment),
		_margin(other._margin),
		_actualWidth(other._actualWidth),
		_actualHeight(other._actualHeight),
		_position(other._position),
		_actualPosition(other._actualPosition)
	{}

	UIElement::UIElement(UIElement&& other) noexcept:
		_cursor(other._cursor),
		_backgroundColor(other._backgroundColor),
		_backgroundTexture(std::move(other._backgroundTexture)),
		_width(other._width),
		_height(other._height),
		_minWidth(other._minWidth),
		_minHeight(other._minHeight),
		_maxWidth(other._maxWidth),
		_maxHeight(other._maxHeight),
		_zIndex(other._zIndex),
		_isVisible(other._isVisible),
		_verticalAlignment(other._verticalAlignment),
		_horizontalAlignment(other._horizontalAlignment),
		_margin(other._margin),
		_actualWidth(other._actualWidth),
		_actualHeight(other._actualHeight),
		_position(other._position),
		_actualPosition(other._actualPosition)
	{}

	void UIElement::SetTooltip(const ToolTip& tooltip)
	{}

	void UIElement::OnRender(RenderContext rc) const
	{		
		
	}
}