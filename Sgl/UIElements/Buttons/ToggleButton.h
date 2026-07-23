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
		ToggleButton(ToggleButton&& other) noexcept;

		void SetIsChecked(bool value, ValueSource source = ValueSource::Local);
		bool IsChecked() const { return _isChecked; }

		static inline StyleableProperty IsCheckedProperty { &SetIsChecked, &IsChecked };
		static inline const PseudoClass OnChecked = PseudoClass::Register("checked");
	protected:
		virtual void OnCheckedChanged() {}
		void OnClick() override;
		void ToggleState();
		void Check();
		void Uncheck();
	private:
		bool _isChecked = false;
		ValueSource _isCheckedSource {};
	};
}