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
		ToggleButton(const ToggleButton&) = delete;
		ToggleButton(ToggleButton&& other) noexcept;

		static const PseudoClassId OnChecked;

		void SetIsChecked(bool value, ValueSource source = ValueSource::Local);
		bool IsChecked() const { return _isChecked; }

		static inline StyleableProperty IsCheckedProperty { &SetIsChecked, &IsChecked };
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