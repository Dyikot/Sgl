#pragma once

#include "../../UIElement/ContentUIElement.h"

namespace Sgl
{
	using Command = Action<const Any&>;
}

namespace Sgl::UIElements
{
	enum class ClickMode
	{
		Release, Press
	};

	class Button : public ContentUIElement
	{
	private:
		using ButtonEventHandler = EventHandler<Button>;

		bool _isPressed = false;

		ClickMode _clickMode = ClickMode::Release;
		Command _command;
		Any _commandParameter;

		ValueSource _clickModeSource {};
		ValueSource _commandSource {};
		ValueSource _commandParameterSource {};
	public:
		Event<ButtonEventHandler> Click;
	public:
		Button();
		Button(const Button& other);
		Button(Button&& other) noexcept;

		void SetClickMode(ClickMode value, ValueSource source = ValueSource::Local);
		ClickMode GetClickMode() const { return _clickMode; }

		void SetCommand(const Command& value, ValueSource sorce = ValueSource::Local);
		const Command& GetCommand() const { return _command; }

		void SetCommandParameter(const Any& value, ValueSource source = ValueSource::Local);
		const Any& GetCommandParameter() const { return _commandParameter; }

		bool IsPressed() const noexcept { return _isPressed; }

		void Render(RenderContext context) final;
	protected:
		virtual void OnClick();
		void OnMouseDown(MouseButtonEventArgs e) override;
		void OnMouseUp(MouseButtonEventArgs e) override;
	public:
		static inline StyleableProperty ClickModeProperty { &SetClickMode, &GetClickMode };
		static inline StyleableProperty CommandProperty { &SetCommand, &GetCommand };
		static inline StyleableProperty CommandParameterProperty { &SetCommandParameter, &GetCommandParameter };
	};
}