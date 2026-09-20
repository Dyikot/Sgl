#include "ToggleButton.h"

namespace Sgl::UIElements
{
	void ToggleButton::SetIsChecked(bool value, ValueSource source)
	{
		SetProperty(IsCheckedProperty, _isChecked, value, _isCheckedSource, source);
	}

	void ToggleButton::OnPropertyChanged(PropertyBase& property)
	{
		Button::OnPropertyChanged(property);

		if(property == IsCheckedProperty)
		{
			PseudoClasses.Set(OnChecked, _isChecked);
			OnCheckedChanged();
		}
	}

	void ToggleButton::OnClick()
	{
		Button::OnClick();
		ToggleState();
	}

	void ToggleButton::ToggleState()
	{
		SetIsChecked(!_isChecked, _isCheckedSource);
	}

	void ToggleButton::Check()
	{
		SetIsChecked(true, _isCheckedSource);
	}

	void ToggleButton::Uncheck()
	{
		SetIsChecked(false, _isCheckedSource);
	}
}
