#include "UIElement.h"

namespace Sgl::UI
{
	UIElement::UIElement():
		Class(*this),
		OnHover(Class),
		OnMousePressed(Class),
		_position()
	{}

	UIElement::UIElement(const UIElement& other):
		base(other),
		Class(other.Class),
		OnHover(other.OnHover),
		OnMousePressed(other.OnMousePressed),
		KeyUp(other.KeyUp),
		KeyDown(other.KeyDown),
		MouseDown(other.MouseDown),
		MouseUp(other.MouseUp),
		MouseMove(other.MouseMove),
		MouseEnter(other.MouseEnter),
		MouseLeave(other.MouseLeave),
		MouseWheel(other.MouseWheel),
		Width(other.Width),
		Height(other.Height),
		MinWidth(other.MinWidth),
		MinHeight(other.MinHeight),
		MaxWidth(other.MaxWidth),
		MaxHeight(other.MaxHeight),
		ZIndex(other.ZIndex),
		Tooltip(other.Tooltip),
		Visibility(other.Visibility),
		VerticalAlignment(other.VerticalAlignment),
		HorizontalAlignment(other.HorizontalAlignment),
		Margin(other.Margin),
		_position()
	{}

	UIElement::UIElement(UIElement&& other) noexcept:
		base(std::move(other)),
		Class(std::move(other.Class)),
		OnHover(std::move(other.OnHover)),
		OnMousePressed(std::move(other.OnMousePressed)),
		KeyUp(std::move(other.KeyUp)),
		KeyDown(std::move(other.KeyDown)),
		MouseDown(std::move(other.MouseDown)),
		MouseUp(std::move(other.MouseUp)),
		MouseMove(std::move(other.MouseMove)),
		MouseEnter(std::move(other.MouseEnter)),
		MouseLeave(std::move(other.MouseLeave)),
		MouseWheel(std::move(other.MouseWheel)),
		Width(other.Width),
		Height(other.Height),
		MinWidth(other.MinWidth),
		MinHeight(other.MinHeight),
		MaxWidth(other.MaxWidth),
		MaxHeight(other.MaxHeight),
		ZIndex(other.ZIndex),
		Tooltip(std::exchange(other.Tooltip, nullptr)),
		Visibility(other.Visibility),
		VerticalAlignment(other.VerticalAlignment),
		HorizontalAlignment(other.HorizontalAlignment),
		Margin(other.Margin),
		_position(other._position)
	{}

	void UIElement::OnRender(RenderContext rc) const
	{		
		if(OnHover.IsActive() && Tooltip)
		{
			Tooltip->OnRender(rc);
		}
	}

	void UIElement::ResetStyle()
	{
		base::ResetStyle();

		Width = 0;
		Height = 0;
		MinWidth = 0;
		MinHeight = 0;
		MaxWidth = std::numeric_limits<float>::max();
		MaxHeight = std::numeric_limits<float>::max();
		ZIndex = 1;
		Tooltip = nullptr;
		Visibility = Visibility::Visible;
		VerticalAlignment = VerticalAlignment::Top;
		HorizontalAlignment = HorizontalAlignment::Left;
		Margin = {};
	}
}