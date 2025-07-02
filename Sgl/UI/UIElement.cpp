#include "UIElement.h"

namespace Sgl
{
	UIElement::UIElement():
		BackgroundColor(Colors::Black),
		Cursor(Cursors::Arrow),
		Texture(),
		Width(*this),
		Height(*this),
		MinWidth(),
		MinHeight(),
		MaxWidth(std::numeric_limits<float>::max()),
		MaxHeight(std::numeric_limits<float>::max()),
		IsVisible(true),
		ZIndex(1),
		Margin(),
		VerticalAlignment(VerticalAlignment::Top),
		HorizontalAlignment(HorizontalAlignment::Left),
		_actualWidth(Width - Margin->Right),
		_actualHeight(Height - Margin->Bottom),
		_position(),
		_actualPosition(_position.x + Margin->Left, _position.y + Margin->Right)
	{}

	UIElement::UIElement(const UIElement& other):
		BackgroundColor(other.BackgroundColor),
		Cursor(other.Cursor),
		Texture(other.Texture),
		Width(other.Width),
		Height(other.Height),
		MinWidth(other.MinWidth),
		MinHeight(other.MinHeight),
		MaxWidth(other.MaxWidth),
		MaxHeight(other.MaxHeight),
		IsVisible(other.IsVisible),
		ZIndex(other.ZIndex),
		Margin(other.Margin),
		VerticalAlignment(other.VerticalAlignment),
		HorizontalAlignment(other.HorizontalAlignment),
		_actualWidth(other._actualWidth),
		_actualHeight(other._actualHeight),
		_position(other._position),
		_actualPosition(other._actualPosition)
	{}

	UIElement::UIElement(UIElement&& other) noexcept:
		BackgroundColor(other.BackgroundColor),
		Cursor(other.Cursor),
		Texture(std::move(other.Texture)),
		Width(other.Width),
		Height(other.Height),
		MinWidth(other.MinWidth),
		MinHeight(other.MinHeight),
		MaxWidth(other.MaxWidth),
		MaxHeight(other.MaxHeight),
		IsVisible(other.IsVisible),
		ZIndex(other.ZIndex),
		Margin(other.Margin),
		VerticalAlignment(other.VerticalAlignment),
		HorizontalAlignment(other.HorizontalAlignment),
		_actualWidth(other._actualWidth),
		_actualHeight(other._actualHeight),
		_position(other._position),
		_actualPosition(other._actualPosition)
	{}

	void UIElement::SetTooltip(const ToolTip& tooltip)
	{}

	void Sgl::UIElement::OnRender(RenderContext context) const
	{		
		
	}
}