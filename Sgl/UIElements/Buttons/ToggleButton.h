#pragma once

#include "Button.h"

namespace Sgl::UIElements
{
	class ToggleButton : public Button
	{
	private:
		using ToggleButtonEventHandler = EventHandler<ToggleButton>;		
	public:
		ToggleButton() = default;

		Event<ToggleButtonEventHandler> IsCheckedChanged;

		void SetIsChecked(bool value);
		bool IsChecked() const { return _isChecked; }

		static inline const VisualState OnChecked = VisualState::Register("checked");
		static inline StyleableProperty IsCheckedProperty { &SetIsChecked, &IsChecked };
	protected:
		void OnPropertyChanged(PropertyBase& property) override;
		Ref<Style> CreateDefaultStyle() const override;
		void OnClick() override;
		void ToggleState();
		void Check();
		void Uncheck();
		virtual void OnIsCheckedChanged();
	private:
		bool _isChecked = false;
		ValueSource _isCheckedSource {};
	};

	class ToggleButtonStyle : public ButtonStyle
	{
	public:
		void OnState(StyleContext& context, const VisualStateSet& states) override;
	protected:
		void OnHover(StyleContext& context) override {}
		void OnPressed(StyleContext& context) override {}
		void OnFocus(StyleContext& context) override {}
		virtual void OnChecked(StyleContext& context) {}
	};
}