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
	public:
		Event<ButtonEventHandler> Click;
	public:
		Button();
		Button(const Button& other);
		Button(Button&& other) noexcept;

		void SetClickMode(ClickMode value);
		ClickMode GetClickMode() const { return _clickMode; }

		void SetCommand(const Command& value);
		const Command& GetCommand() const { return _command; }

		void SetCommandParameter(const Any& value);
		const Any& GetCommandParameter() const { return _commandParameter; }

		bool IsPressed() const noexcept { return _isPressed; }

		void Render(RenderContext context) final;
		void ApplyStyle() override;
	protected:
		virtual void OnClick();
		void OnMouseDown(MouseButtonEventArgs e) override;
		void OnMouseUp(MouseButtonEventArgs e) override;
	public:
		static inline SglProperty ClickModeProperty { &SetClickMode, &GetClickMode };
		static inline SglProperty CommandProperty { &SetCommand, &GetCommand };
		static inline SglProperty CommandParameterProperty { &SetCommandParameter, &GetCommandParameter };
	};
}