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
		ToggleButton(const ToggleButton& other);
		ToggleButton(ToggleButton&& other) noexcept;

		Event<ToggleButtonEventHandler> CheckedChanged;

		void SetIsChecked(bool value, ValueSource source = ValueSource::Local);
		bool IsChecked() const { return _isChecked; }

		static inline StyleableProperty IsCheckedProperty { &SetIsChecked, &IsChecked };
	protected:
		virtual void OnCheckChanged();
		void OnClick() override;
	private:
		bool _isChecked = false;
		ValueSource _isCheckedSource {};
	};
}