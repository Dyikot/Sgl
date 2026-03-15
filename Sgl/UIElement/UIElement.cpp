#include "UIElement.h"

namespace Sgl
{
	const PseudoClassId UIElement::OnHover = PseudoClassesRegistry::Register("hover");
	const PseudoClassId UIElement::OnPressed = PseudoClassesRegistry::Register("pressed");

	class UIElementBackgroundRenderer
	{
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
	private:
		RenderContext _context;
		FRect _rect;
	};

	UIElement::UIElement(const UIElement& other):
		Layoutable(other),
		_tag(other._tag)
	{}

	UIElement::UIElement(UIElement&& other) noexcept:
		Layoutable(std::move(other)),
		_tag(std::move(other._tag))
	{}

	void UIElement::SetTag(const Any& value, ValueSource source)
	{
		SetProperty(TagProperty, _tag, value, _tagSource, source);
	}

	void UIElement::RenderBackground(RenderContext context)
	{
		std::visit(UIElementBackgroundRenderer(context, _bounds), GetBackground());
	}

	void UIElement::OnCursorChanged(const Cursor& cursor)
	{
		if(IsMouseOver())
		{
			Cursor::Set(cursor);
		}
	}

	void UIElement::InheritProperties(StyleableElement& parent)
	{
		SetDataContext(parent.GetDataContext(), ValueSource::Inheritance);
		SetCursor(static_cast<Renderable&>(parent).GetCursor(), ValueSource::Inheritance);
	}

	void UIElement::OnAttachedToLogicalTree()
	{
		Layoutable::OnAttachedToLogicalTree();

		auto& parent = static_cast<StyleableElement&>(*GetStylingParent());
		InheritProperties(parent);
		ApplyBindings();
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
		if(e.Button == MouseButton::Left)
		{
			PseudoClasses.Set(OnPressed);
		}

		MouseDown(*this, e);
	}

	void UIElement::OnMouseUp(MouseButtonEventArgs e)
	{
		if(e.Button == MouseButton::Left)
		{
			PseudoClasses.Reset(OnPressed);
		}

		MouseUp(*this, e);
	}

	void UIElement::OnMouseWheelChanged(MouseWheelEventArgs& e)
	{
		MouseWheel(*this, e);
	}

	void UIElement::OnMouseEnter(MouseMoveEventArgs e)
	{
		Cursor::Set(GetCursor());
		PseudoClasses.Set(OnHover);
		MouseEnter(*this, e);
	}

	void UIElement::OnMouseLeave(MouseMoveEventArgs e)
	{
		MouseLeave(*this, e);
		PseudoClasses.Reset(OnHover);
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