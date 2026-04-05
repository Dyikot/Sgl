#include "UIElement.h"

namespace Sgl
{
	const PseudoClassId UIElement::OnHover = PseudoClassesRegistry::Register("hover");
	const PseudoClassId UIElement::OnPressed = PseudoClassesRegistry::Register("pressed");

	UIElement::UIElement(UIElement&& other) noexcept:
		Layoutable(std::move(other)),
		_tag(std::move(other._tag))
	{}

	void UIElement::SetTag(const Any& value, ValueSource source)
	{
		SetProperty(TagProperty, _tag, value, _tagSource, source);
	}

	void UIElement::OnCursorChanged(const Cursor& cursor)
	{
		if(IsMouseOver())
		{
			Cursor::Set(cursor);
		}
	}

	void UIElement::OnAttachedToLogicalTree()
	{
		Layoutable::OnAttachedToLogicalTree();

		auto& parent = static_cast<StyleableElement&>(*GetStylingParent());
		SetDataContext(parent.GetDataContext(), ValueSource::Inheritance);
		SetCursor(static_cast<Renderable&>(parent).GetCursor(), ValueSource::Inheritance);

		ApplyBindings();
	}

	void UIElement::OnKeyUp(KeyEventArgs e)
	{
		KeyUp.Invoke(*this, e);
	}

	void UIElement::OnKeyDown(KeyEventArgs e)
	{
		KeyDown.Invoke(*this, e);
	}

	void UIElement::OnMouseMove(MouseMoveEventArgs e)
	{
		MouseMove.Invoke(*this, e);
	}

	void UIElement::OnMouseDown(MouseButtonEventArgs e)
	{
		if(e.Button == MouseButton::Left)
		{
			PseudoClasses.Set(OnPressed);
		}

		MouseDown.Invoke(*this, e);
	}

	void UIElement::OnMouseUp(MouseButtonEventArgs e)
	{
		if(e.Button == MouseButton::Left)
		{
			PseudoClasses.Reset(OnPressed);
		}

		MouseUp.Invoke(*this, e);
	}

	void UIElement::OnMouseWheelChanged(MouseWheelEventArgs& e)
	{
		MouseWheel.Invoke(*this, e);
	}

	void UIElement::OnMouseEnter(MouseMoveEventArgs e)
	{
		Cursor::Set(GetCursor());
		PseudoClasses.Set(OnHover);
		MouseEnter.Invoke(*this, e);
	}

	void UIElement::OnMouseLeave(MouseMoveEventArgs e)
	{
		MouseLeave.Invoke(*this, e);
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