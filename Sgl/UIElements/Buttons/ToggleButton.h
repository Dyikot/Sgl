#pragma once

#include "Button.h"

namespace Sgl::UIElements
{
	class ToggleButton : public Button
	{
	public:
		using ToggleButtonEventHandler = EventHandler<ToggleButton>;

		Event<ToggleButtonEventHandler> CheckedChanged;
	private:
		bool _isChecked = false;

		ValueSource _isCheckedSource {};
	public:
		ToggleButton() = default;
		ToggleButton(const ToggleButton& other);
		ToggleButton(ToggleButton&& other) noexcept;

		void SetIsChecked(bool value);
		void SetIsChecked(bool value, ValueSource source);
		bool IsChecked() const { return _isChecked; }
	protected:
		virtual void OnCheckChanged();
		void OnClick() override;
	public:
		static inline StyleableProperty IsCheckedProperty { &SetIsChecked, &IsChecked };
	};
}