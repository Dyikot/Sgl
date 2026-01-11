#include "Button.h"

namespace Sgl::UIElements
{
	Button::Button()
	{
		SetCursor(Cursors::Pointer);
		SetPadding(Thickness(10), ValueSource::Default);
		SetBackground(Colors::LightGray, ValueSource::Default);
	}

	Button::Button(const Button& other):
		ContentUIElement(other),
		_isPressed(other._isPressed),
		_clickMode(other._clickMode)
	{}

	Button::Button(Button&& other) noexcept:
		ContentUIElement(std::move(other)),
		_isPressed(other._isPressed),
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

	void Button::Render(RenderContext context)
	{
		RenderBackground(context);
		ContentUIElement::Render(context);
	}

	void Button::OnClick()
	{
		Click(*this);

		if(_command.HasTarget())
		{
			_command(_commandParameter);
		}
	}

	void Button::OnMouseDown(MouseButtonEventArgs e)
	{
		ContentUIElement::OnMouseDown(e);

		if(e.Button == MouseButton::Left)
		{
			_isPressed = true;

			if(_clickMode == ClickMode::Press)
			{
				OnClick();
			}
		}
	}

	void Button::OnMouseUp(MouseButtonEventArgs e)
	{
		ContentUIElement::OnMouseUp(e);

		if(e.Button == MouseButton::Left)
		{
			_isPressed = false;

			if(_clickMode == ClickMode::Release)
			{
				OnClick();
			}
		}
	}
}