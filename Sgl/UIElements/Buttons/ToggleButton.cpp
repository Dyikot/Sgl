#include "ToggleButton.h"

namespace Sgl::UIElements
{
	const PseudoClassId ToggleButton::OnChecked = PseudoClassesRegistry::Register("checked");

	ToggleButton::ToggleButton(const ToggleButton& other):
		Button(other)
	{}

	ToggleButton::ToggleButton(ToggleButton&& other) noexcept:
		Button(std::move(other))
	{}

	void ToggleButton::SetIsChecked(bool value, ValueSource source)
	{
		bool isChecked = PseudoClasses.Has(OnChecked);

		if(SetProperty(IsCheckedProperty, isChecked, value, _isCheckedSource, source))
		{
			PseudoClasses.Set(OnChecked, value);
		}
	}

	void ToggleButton::OnClick()
	{
		Button::OnClick();
		SetIsChecked(!PseudoClasses.Has(OnChecked), _isCheckedSource);
	}
}
