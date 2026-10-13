#include "Button.h"
#include "../../Styling/StyleContext.h"

namespace Sgl::UIElements
{
	Button::Button()
	{
		SetName("Button");
		SetIsFocusable(true);
	}

	void Button::SetClickMode(ClickMode value)
	{
		SetProperty(ClickModeProperty, _clickMode, value, _clickModeSource);
	}

	void Button::SetCommand(const Command& value)
	{
		SetProperty(CommandProperty, _command, value, _commandSource);
	}

	void Button::SetCommandParameter(const Any& value)
	{
		SetProperty(CommandParameterProperty, _commandParameter, value, _commandParameterSource);
	}

	Ref<Style> Button::CreateDefaultStyle() const
	{
		return New<ButtonStyle>();
	}

	void Button::OnMouseDown(MouseClickEventArgs& e)
	{
		ContentUIElement::OnMouseDown(e);

		if(e.Button == MouseButton::Left)
		{
			if(_clickMode == ClickMode::Press)
			{
				OnClick();
			}

			e.Handled = true;
		}
	}

	void Button::OnMouseUp(MouseClickEventArgs& e)
	{
		ContentUIElement::OnMouseUp(e);

		if(e.Button == MouseButton::Left)
		{
			if(_clickMode == ClickMode::Release)
			{
				OnClick();
			}

			e.Handled = true;
		}
	}

	void Button::OnKeyDown(KeyEventArgs& e)
	{
		UIElement::OnKeyDown(e);

		if(e.Key == KeyCodes::Return || e.Key == KeyCodes::KpEnter)
		{
			States.Set(OnPressed);
			OnClick();
			e.Handled = true;
		}
	}

	void Button::OnKeyUp(KeyEventArgs& e)
	{
		UIElement::OnKeyUp(e);

		if(e.Key == KeyCodes::Return || e.Key == KeyCodes::KpEnter)
		{
			States.Reset(OnPressed);
			e.Handled = true;
		}
	}

	void Button::OnClick()
	{
		Click.Invoke(*this);

		if(_command.HasTarget())
		{
			_command(_commandParameter);
		}
	}

	void ButtonStyle::OnBase(StyleContext& context)
	{
		context.Set(Button::CursorProperty, Cursors::Pointer);
		context.Set(Button::BackgroundProperty, Colors::Grey_2);
	}

	void ButtonStyle::OnHover(StyleContext& context)
	{
		context.Set(Button::BackgroundProperty, Colors::Grey_3);
	}

	void ButtonStyle::OnPressed(StyleContext& context)
	{
		context.Set(Button::BackgroundProperty, Colors::Grey_4);
	}

	void ButtonStyle::OnFocus(StyleContext& context)
	{
	
	}
}