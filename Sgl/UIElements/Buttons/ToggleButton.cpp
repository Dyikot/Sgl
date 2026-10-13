#include "ToggleButton.h"

namespace Sgl::UIElements
{
	void ToggleButton::SetIsChecked(bool value)
	{
		SetProperty(IsCheckedProperty, _isChecked, value, _isCheckedSource);
	}

	void ToggleButton::OnPropertyChanged(PropertyBase& property)
	{
		Button::OnPropertyChanged(property);

		if(property == IsCheckedProperty)
		{
			States.Set(OnChecked, _isChecked);
			OnIsCheckedChanged();
		}
	}

	Ref<Style> ToggleButton::CreateDefaultStyle() const
	{
		return New<ToggleButtonStyle>();
	}

	void ToggleButton::OnClick()
	{
		Button::OnClick();
		ToggleState();
	}

	void ToggleButton::ToggleState()
	{
		SetProperty(IsCheckedProperty, _isChecked, !_isChecked);
	}

	void ToggleButton::Check()
	{
		SetProperty(IsCheckedProperty, _isChecked, true);
	}

	void ToggleButton::Uncheck()
	{
		SetProperty(IsCheckedProperty, _isChecked, false);
	}

	void ToggleButton::OnIsCheckedChanged()
	{
		IsCheckedChanged.Invoke(*this);
	}

	void ToggleButtonStyle::OnState(StyleContext& context, const VisualStateSet& states)
	{
		ButtonStyle::OnState(context, states);

		if(states.Has(ToggleButton::OnChecked))
		{
			OnChecked(context);
		}
	}
}
