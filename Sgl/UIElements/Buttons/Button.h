#pragma once

#include "../../UIElement/ContentUIElement.h"

namespace Sgl::UIElements
{
	/// <summary>
	/// Represents an action that can be bound to UI elements. For example to buttons.
	/// </summary>
	using Command = Action<const Any&>;

	/// <summary>
	/// Specifies when a click event is generated for interactive controls (e.g., buttons).
	/// </summary>
	enum class ClickMode
	{
		Release, // The Click event is triggered when the mouse button is released over the control.
		Press    // The Click event is triggered as soon as the mouse button is pressed on the control.
	};

	class Button : public ContentUIElement
	{
	private:
		using ButtonEventHandler = EventHandler<Button>;
	public:
		Button();
		Button(const Button& other);
		Button(Button&& other) noexcept;

		Event<ButtonEventHandler> Click;

		void SetClickMode(ClickMode value, ValueSource source = ValueSource::Local);
		ClickMode GetClickMode() const { return _clickMode; }

		void SetCommand(const Command& value, ValueSource sorce = ValueSource::Local);
		const Command& GetCommand() const { return _command; }

		void SetCommandParameter(const Any& value, ValueSource source = ValueSource::Local);
		const Any& GetCommandParameter() const { return _commandParameter; }

		bool IsPressed() const noexcept { return _isPressed; }

		void Render(RenderContext context) final;

		static inline StyleableProperty ClickModeProperty { &SetClickMode, &GetClickMode };
		static inline StyleableProperty CommandProperty { &SetCommand, &GetCommand };
		static inline StyleableProperty CommandParameterProperty { &SetCommandParameter, &GetCommandParameter };
	protected:
		virtual void OnClick();
		void OnMouseDown(MouseButtonEventArgs e) override;
		void OnMouseUp(MouseButtonEventArgs e) override;
	private:
		bool _isPressed = false;
		ClickMode _clickMode = ClickMode::Release;
		Command _command;
		Any _commandParameter;

		ValueSource _clickModeSource {};
		ValueSource _commandSource {};
		ValueSource _commandParameterSource {};
	};
}