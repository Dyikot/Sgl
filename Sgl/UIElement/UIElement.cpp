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

	const PseudoClassId UIElement::HoverPseudoClass = PseudoClassesRegistry::Register("hover");

	UIElement::UIElement(const UIElement& other):
		Layoutable(other),
		_isMouseOver(other._isMouseOver),
		_tag(other._tag),
		_tooltip(other._tooltip)
	{}

	UIElement::UIElement(UIElement&& other) noexcept:
		Layoutable(std::move(other)),
		_isMouseOver(other._isMouseOver),
		_tag(std::move(other._tag)),
		_tooltip(std::move(other._tooltip))
	{}

	void UIElement::SetTag(const Any& value, ValueSource source)
	{
		SetProperty(TagProperty, _tag, value, _tagSource, source);
	}

	void UIElement::SetToolTip(const Ref<UIElement>& value, ValueSource source)
	{
		if(SetProperty(ToolTipProperty, _tooltip, value, _tooltipSource, source))
		{
			if(_isMouseOver)
			{
				InvalidateRender();
			}
		}
	}

	void UIElement::Render(RenderContext context)
	{		
		if(_tooltip && _tooltip->IsVisible())
		{
			_tooltip->Render(context);
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

	void UIElement::InheritProperties(StyleableElement& parent)
	{
		SetDataContext(parent.GetDataContext(), ValueSource::Inheritance);
		SetCursor(static_cast<Renderable&>(parent).GetCursor(), ValueSource::Inheritance);
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
		PseudoClasses.set(HoverPseudoClass);
		MouseEnter(*this, e);
	}

	void UIElement::OnMouseLeave(MouseMoveEventArgs e)
	{
		MouseLeave(*this, e);
		PseudoClasses.reset(HoverPseudoClass);
		_isMouseOver = false;
	}

	void UIElement::OnAttachedToLogicalTree()
	{
		Layoutable::OnAttachedToLogicalTree();

		auto& parent = static_cast<StyleableElement&>(*GetStylingParent());
		InheritProperties(parent);
		ApplyBindings();
	}

	Ref<UIElement> UIElementDataTemplate::Build(const Ref<INotifyPropertyChanged>& data)
	{
		return data.As<UIElement>();
	}

	bool UIElementDataTemplate::Match(const Ref<INotifyPropertyChanged>& data) const
	{
		return data.Is<UIElement>();
	}
}