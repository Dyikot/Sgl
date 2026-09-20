#pragma once

#include "../ContentUIElement.h"

namespace Sgl::UIElements
{
	//! @brief Represents an action that can be bound to UI elements
	using Command = Action<const Any&>;

	//! @brief Specifies when a click event is generated for interactive controls (e.g., buttons)
	enum class ClickMode
	{
		Release, // The Click event is triggered when the mouse button is released over the control
		Press    // The Click event is triggered as soon as the mouse button is pressed on the control
	};

	class Button : public ContentUIElement
	{
	private:
		using ButtonEventHandler = EventHandler<Button, EventArgs>;
	public:
		Button();

		Event<ButtonEventHandler> Click;

		void SetClickMode(ClickMode value, ValueSource source = ValueSource::Local);
		ClickMode GetClickMode() const { return _clickMode; }

		void SetCommand(const Command& value, ValueSource sorce = ValueSource::Local);
		const Command& GetCommand() const { return _command; }

		void SetCommandParameter(const Any& value, ValueSource source = ValueSource::Local);
		const Any& GetCommandParameter() const { return _commandParameter; }

		static inline StyleableProperty ClickModeProperty { &SetClickMode, &GetClickMode };
		static inline StyleableProperty CommandProperty { &SetCommand, &GetCommand };
		static inline StyleableProperty CommandParameterProperty { &SetCommandParameter, &GetCommandParameter };
	protected:
		void OnMouseDown(MouseClickEventArgs& e) override;
		void OnMouseUp(MouseClickEventArgs& e) override;
		void OnKeyDown(KeyEventArgs e) override;
		void OnKeyUp(KeyEventArgs e) override;
		virtual void OnClick();
	private:
		ClickMode _clickMode = ClickMode::Release;
		Command _command;
		Any _commandParameter;

		ValueSource _clickModeSource {};
		ValueSource _commandSource {};
		ValueSource _commandParameterSource {};
	};
}