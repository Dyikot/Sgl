#include "UIElement.h"

#include "../Window.h"

namespace Sgl
{
	class UIElementBackgroundRenderer
	{
	private:
		RenderContext _context;
		FRect _rect;
	public:
		UIElementBackgroundRenderer(RenderContext context, FRect rect):
			_context(context), _rect(rect) 
		{}

		void operator()(Color color)
		{
			if(!color.IsTransparent())
			{
				_context.DrawRectangleFill(_rect, color);
			}
		}

		void operator()(const Texture& texture)
		{
			_context.DrawTexture(texture, _rect);
		}
	};

	UIElement::UIElement(const UIElement& other):
		Layoutable(other),
		_isMouseOver(other._isMouseOver),
		_tag(other._tag),
		_toolTip(other._toolTip)
	{}

	UIElement::UIElement(UIElement&& other) noexcept:
		Layoutable(std::move(other)),
		_isMouseOver(other._isMouseOver),
		_tag(std::move(other._tag)),
		_toolTip(std::move(other._toolTip))
	{}

	void UIElement::SetTag(const Any& value)
	{
		SetProperty(TagProperty, _tag, value);
	}	

	void UIElement::SetToolTip(const Ref<UIElement>& value)
	{
		if(SetProperty(ToolTipProperty, _toolTip, value) && _isMouseOver)
		{
			InvalidateRender();
		}
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
		std::visit(UIElementBackgroundRenderer(context, _bounds), GetBackground());
	}

	void UIElement::OnCursorChanged(const Cursor& cursor)
	{
		if(_isMouseOver)
		{
			Cursor::Set(cursor);
		}
	}

	void UIElement::OnKeyUp(KeyEventArgs e)
	{
		KeyUp(*this, e);
	}

	void UIElement::OnKeyDown(KeyEventArgs e)
	{
		KeyDown(*this, e);
	}

	void UIElement::OnMouseMove(MouseMoveEventArgs e)
	{
		MouseMove(*this, e);
	}

	void UIElement::OnMouseDown(MouseButtonEventArgs e)
	{
		MouseDown(*this, e);
	}

	void UIElement::OnMouseUp(MouseButtonEventArgs e)
	{
		MouseUp(*this, e);
	}

	void UIElement::OnMouseWheelChanged(MouseWheelEventArgs& e)
	{
		MouseWheel(*this, e);
	}

	void UIElement::OnMouseEnter(MouseMoveEventArgs e)
	{
		_isMouseOver = true;
		Cursor::Set(GetCursor());
		MouseEnter(*this, e);
	}

	void UIElement::OnMouseLeave(MouseMoveEventArgs e)
	{
		MouseLeave(*this, e);
		_isMouseOver = false;
	}

	void UIElement::OnAttachedToLogicalTree()
	{
		Layoutable::OnAttachedToLogicalTree();
		InvalidateMeasure();
	}

	void UIElement::OnDetachedFromLogicalTree()
	{
		Layoutable::OnDetachedFromLogicalTree();
		InvalidateMeasure();
	}

	Ref<UIElement> UIElementDataTemplate::Build(const Ref<INotityPropertyChanged>& data)
	{
		return data.As<UIElement>();
	}

	bool UIElementDataTemplate::Match(const Ref<INotityPropertyChanged>& data) const
	{
		return data.Is<UIElement>();
	}
}