#include "UIElement.h"

namespace Sgl::UI
{
	UIElement::UIElement():
		ClassStyle(*this),
		OnHover(ClassStyle),
		OnMousePressed(ClassStyle)
	{}

	UIElement::UIElement(const UIElement& other):
		ClassStyle(other.ClassStyle),
		OnHover(other.OnHover),
		OnMousePressed(other.OnMousePressed),
		KeyUp(other.KeyUp),
		KeyDown(other.KeyDown),
		MouseDown(other.MouseDown),
		MouseUp(other.MouseUp),
		MouseMove(other.MouseMove),
		MouseEnter(other.MouseEnter),
		MouseLeave(other.MouseLeave),
		MouseDoubleClick(other.MouseDoubleClick),
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
		Padding(other.Padding),
		Border(other.Border)
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
		Padding = {};
		Border = {};
	}
}