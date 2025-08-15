#include "UIElement.h"

namespace Sgl
{
	UIElement::UIElement(const UIElement& other):
		Layoutable(other),
		_isMouseOver(other._isMouseOver),
		_cursor(other._cursor),
		_background(other._background),
		_tag(other._tag),
		_toolTip(other._toolTip),
		_zIndex(other._zIndex)
	{}

	UIElement::UIElement(UIElement&& other) noexcept:
		Layoutable(std::move(other)),
		_isMouseOver(other._isMouseOver),
		_cursor(other._cursor),
		_background(std::move(other._background)),
		_tag(std::move(other._tag)),
		_toolTip(std::move(other._toolTip)),
		_zIndex(other._zIndex)
	{}

	void UIElement::Render(RenderContext context)
	{		
		if(_toolTip && _toolTip->IsVisible())
		{
			_toolTip->Render(context);
		}
	}

	void UIElement::RenderBackground(RenderContext context) const
	{
		switch(_background.GetType())
		{
			case Brush::Color:
			{
				if(auto color = _background.AsColor(); !color.IsTransparent())
				{
					context.DrawRectangleFill(_bounds, color);
				}

				break;
			}

			case Brush::Texture:
			{
				context.DrawTexture(_background.AsTexture(), _bounds);
				break;
			}
		}
	}
}