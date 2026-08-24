#include "Button.h"

namespace Sgl::UIElements
{
	Button::Button()
	{
		Name = "Button";
		SetCursor(Cursors::Pointer);
		SetBackground(Colors::LightGray, ValueSource::Default);
		SetIsFocusable(true, ValueSource::Default);
	}

	Button::Button(Button&& other) noexcept:
		ContentUIElement(std::move(other)),
		_clickMode(other._clickMode),
		_command(std::move(other._command)),
		_commandParameter(std::move(other._commandParameter))
	{}

	void Button::SetClickMode(ClickMode value, ValueSource source)
	{
		SetProperty(ClickModeProperty, _clickMode, value, _clickModeSource, source);
	}

	void Button::SetCommand(const Command& value, ValueSource sorce)
	{
		SetProperty(CommandProperty, _command, value, _commandSource, sorce);
	}

	void Button::SetCommandParameter(const Any& value, ValueSource source)
	{
		SetProperty(CommandParameterProperty, _commandParameter, value, _commandParameterSource, source);
	}

	void Button::OnClick()
	{
		Click.Invoke(*this);

		if(_command.HasTarget())
		{
			_command(_commandParameter);
		}
	}

	void Button::OnMouseDown(MouseClickEventArgs& e)
	{
		ContentUIElement::OnMouseDown(e);

		if(e.Button == MouseButton::Left && _clickMode == ClickMode::Press)
		{
			OnClick();
		}
	}

	void Button::OnMouseUp(MouseClickEventArgs& e)
	{
		ContentUIElement::OnMouseUp(e);

		if(e.Button == MouseButton::Left && _clickMode == ClickMode::Release)
		{
			OnClick();
		}
	}

	void Button::OnKeyDown(KeyEventArgs e)
	{
		UIElement::OnKeyDown(e);

		if(e.Key == KeyCodes::Return || e.Key == KeyCodes::KpEnter)
		{
			PseudoClasses.Set(OnPressed);
			OnClick();
		}
	}

	void Button::OnKeyUp(KeyEventArgs e)
	{
		UIElement::OnKeyUp(e);

		if(e.Key == KeyCodes::Return || e.Key == KeyCodes::KpEnter)
		{
			PseudoClasses.Reset(OnPressed);
		}
	}
}