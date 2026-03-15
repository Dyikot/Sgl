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

		static const PseudoClassId OnChecked;

		void SetIsChecked(bool value, ValueSource source = ValueSource::Local);
		bool IsChecked() const { return PseudoClasses.Has(OnChecked); }

		static inline StyleableProperty IsCheckedProperty { &SetIsChecked, &IsChecked };
	protected:
		void OnClick() override;
	private:
		ValueSource _isCheckedSource {};
	};
}