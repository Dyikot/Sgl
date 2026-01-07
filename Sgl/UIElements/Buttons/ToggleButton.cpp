#include "ToggleButton.h"

namespace Sgl::UIElements
{
	ToggleButton::ToggleButton(const ToggleButton& other):
		Button(other),
		_isChecked(other._isChecked)
	{}

	ToggleButton::ToggleButton(ToggleButton&& other) noexcept:
		Button(std::move(other)),
		_isChecked(other._isChecked)
	{}

	void ToggleButton::SetIsChecked(bool value, ValueSource source)
	{
		if(SetProperty(IsCheckedProperty, _isChecked, value, _isCheckedSource, source))
		{
			OnCheckChanged();
		}
	}

	void ToggleButton::OnCheckChanged()
	{
		InvalidateContentPresenter();
		CheckedChanged(*this);
	}

	void ToggleButton::OnClick()
	{
		Button::OnClick();
		SetIsChecked(!_isChecked, _isCheckedSource);		
	}
}
