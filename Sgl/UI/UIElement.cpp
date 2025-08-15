#include "UIElement.h"

namespace Sgl
{
	UIElement::UIElement(const UIElement& other):
		Layoutable(other),
		_isMouseOver(other._isMouseOver),
		_tag(other._tag),
		_toolTip(other._toolTip),
		_zIndex(other._zIndex)
	{}

	UIElement::UIElement(UIElement&& other) noexcept:
		Layoutable(std::move(other)),
		_isMouseOver(other._isMouseOver),
		_tag(std::move(other._tag)),
		_toolTip(std::move(other._toolTip)),
		_zIndex(other._zIndex)
	{}

	void UIElement::SetTag(const Any & value)
	{
		SetProperty(TagProperty, _tag, value);
	}

	void UIElement::SetToolTip(Ref<UIElement> value)
	{
		SetProperty(ToolTipProperty, _toolTip, value);

		if(_isMouseOver)
		{
			InvalidateRender();
		}
	}

	void UIElement::SetZIndex(size_t value)
	{
		SetProperty(ZIndexProperty, _zIndex, value);
		InvalidateRender();
	}

	void UIElement::Render(RenderContext context)
	{		
		if(_toolTip && _toolTip->IsVisible())
		{
			_toolTip->Render(context);
		}

		Renderable::Render(context);
	}

	void UIElement::RenderBackground(RenderContext context)
	{
		switch(auto background = GetBackground(); background.GetType())
		{
			case Brush::Color:
			{
				if(auto color = background.AsColor(); !color.IsTransparent())
				{
					context.DrawRectangleFill(_bounds, color);
				}

				break;
			}

			case Brush::Texture:
			{
				context.DrawTexture(background.AsTexture(), _bounds);
				break;
			}
		}
	}

	void UIElement::OnCursorChanged(const Cursor& cursor)
	{
		if(_isMouseOver)
		{
			Cursor::Set(cursor);
		}
	}

	void UIElement::OnKeyUp(const KeyEventArgs& e)
	{
		KeyUp.TryInvoke(*this, e);
	}

	void UIElement::OnKeyDown(const KeyEventArgs& e)
	{
		KeyDown.TryInvoke(*this, e);
	}

	void UIElement::OnMouseMove(const MouseEventArgs& e)
	{
		MouseMove.TryInvoke(*this, e);
	}

	void UIElement::OnMouseDown(const MouseButtonEventArgs& e)
	{
		MouseDown.TryInvoke(*this, e);
	}

	void UIElement::OnMouseUp(const MouseButtonEventArgs& e)
	{
		MouseUp.TryInvoke(*this, e);
	}

	void UIElement::OnMouseWheelChanged(const MouseWheelEventArgs& e)
	{
		MouseWheel.TryInvoke(*this, e);
	}

	void UIElement::OnMouseEnter(const MouseEventArgs& e)
	{
		_isMouseOver = true;
		Cursor::Set(GetCursor());
		MouseEnter.TryInvoke(*this, e);
	}

	void UIElement::OnMouseLeave(const MouseEventArgs& e)
	{
		MouseLeave.TryInvoke(*this, e);
		_isMouseOver = false;
	}
}