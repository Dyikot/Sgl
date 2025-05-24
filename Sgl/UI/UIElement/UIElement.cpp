#include "UIElement.h"

namespace Sgl
{
	UIElement::UIElement():
		ClassStyle(*this),
		OnHover(ClassStyle),
		OnMousePressed(ClassStyle),
		Children(*this)
	{}

	void UIElement::OnRender(RenderContext renderContext) const
	{
		for(auto& [_, element] : Children)
		{
			element.OnRender(renderContext);
		}

		if(_isHover && Tooltip)
		{
			Tooltip->OnRender(renderContext);
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