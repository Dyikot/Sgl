#include "Button.h"

namespace Sgl::UIElements
{
	Button::Button()
	{
		SetPadding(Thickness(10));
		SetCursor(Cursors::Pointer);
		SetBackground(Colors::LightGray);
	}

	Button::Button(const Button& other):
		ContentUIElement(other),
		_isPressed(other._isPressed),
		_clickMode(other._clickMode)
	{}

	Button::Button(Button&& other) noexcept:
		ContentUIElement(std::move(other)),
		_isPressed(other._isPressed),
		_clickMode(other._clickMode)
	{}

	void Button::SetClickMode(ClickMode value)
	{
		SetProperty(ClickModeProperty, _clickMode, value);
	}

	void Button::SetCommand(const Command& value)
	{
		SetProperty(CommandProperty, _command, value);
	}

	void Button::SetCommandParameter(const Any& value)
	{
		SetProperty(CommandParameterProperty, _commandParameter, value);
	}

	void Button::Render(RenderContext context)
	{
		RenderBackground(context);
		ContentUIElement::Render(context);
	}

	void Button::ApplyStyle()
	{
		ContentUIElement::ApplyStyle();

		if(const auto& presenter = GetContentPresenter())
		{
			if(presenter->GetCursor() == Cursors::Arrow)
			{
				presenter->SetCursor(GetCursor());
			}
		}
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