#pragma once

#include "../../UIElement/ContentUIElement.h"
#include "../../Data/Command.h"

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
	public:
		Event<ButtonEventHandler> Click;
	public:
		Button();
		Button(const Button& other);
		Button(Button&& other) noexcept;

		void SetClickMode(ClickMode value);
		ClickMode GetClickMode() const { return _clickMode; }

		void SetCommand(Command value);
		Command GetCommand() const { return _command; }

		void SetCommandParameter(const Any& value);
		const Any& GetCommandParameter() const { return _commandParameter; }

		bool IsPressed() const noexcept { return _isPressed; }

		void Render(RenderContext context) override;
		void ApplyStyle() override;
	protected:
		virtual void OnClick();
		void OnMouseDown(MouseButtonEventArgs e) override;
		void OnMouseUp(MouseButtonEventArgs e) override;
	public:
		static inline ObservableProperty ClickModeProperty { &SetClickMode, &GetClickMode };
		static inline ObservableProperty CommandProperty { &SetCommand, &GetCommand };
		static inline ObservableProperty CommandParameterProperty { &SetCommandParameter, &GetCommandParameter };
	};
}