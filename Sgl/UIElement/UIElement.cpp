#include "UIElement.h"

#include "../Render/BackgroundRenderer.h"
#include "../Window.h"

namespace Sgl
{
	UIElement::UIElement(const UIElement& other):
		Layoutable(other),
		_isMouseOver(other._isMouseOver),
		_tag(other._tag),
		_name(other._name),
		_toolTip(other._toolTip)
	{}

	UIElement::UIElement(UIElement&& other) noexcept:
		Layoutable(std::move(other)),
		_isMouseOver(other._isMouseOver),
		_tag(std::move(other._tag)),
		_name(std::move(other._name)),
		_toolTip(std::move(other._toolTip))
	{}

	void UIElement::SetTag(const Any& value)
	{
		SetProperty(TagProperty, _tag, value);
	}

	void UIElement::SetName(const std::string& value)
	{
		_name = value;
	}

	void UIElement::SetToolTip(Ref<UIElement> value)
	{
		if(SetProperty(ToolTipProperty, _toolTip, value) && _isMouseOver)
		{
			InvalidateRender();
		}
	}

	void UIElement::SetDataContext(Ref<ObservableObject> value)
	{
		SetProperty(DataContextProperty, _dataContext, value);
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

	void UIElement::OnUpdate()
	{
		_hasUpdates = false;
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

	void UIElement::OnMouseWheelChanged(MouseWheelEventArgs e)
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

	void UIElement::OnAttachedToLogicalTree(IStyleHost& parent)
	{
		Layoutable::OnAttachedToLogicalTree(parent);
		InvalidateMeasure();

		if(_hasUpdates)
		{
			auto window = static_cast<Window*>(GetVisualRoot());
			window->RequestUpdate(*this);
		}
	}

	void UIElement::OnDetachedFromLogicalTree()
	{
		InvalidateMeasure();
		Layoutable::OnDetachedFromLogicalTree();
		
		if(_hasUpdates)
		{
			auto window = static_cast<Window*>(GetVisualRoot());
			window->CancelUpdateRequest(*this);
		}
	}

	void UIElement::RequestUpdate()
	{
		if(!_hasUpdates)
		{
			_hasUpdates = true;

			if(auto window = static_cast<Window*>(GetVisualRoot()))
			{
				window->RequestUpdate(*this);
			}
		}		
	}

	Ref<UIElement> UIElementDataTemplate::Build(const Ref<ObservableObject>& data)
	{
		return data.As<UIElement>();
	}

	bool UIElementDataTemplate::Match(const Ref<ObservableObject>& data) const
	{
		return data.Is<UIElement>();
	}
}