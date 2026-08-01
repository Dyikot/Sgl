#include "Slider.h"
#include "../Buttons/RepeatButton.h"

namespace Sgl::UIElements
{
	Slider::Slider()
	{
		Name = "Slider";
		BuildTemplate();
	}

	void Slider::OnValueChanged(float value)
	{
		RangeBase::OnValueChanged(value);
		_track->SetFillRatio(GetRelativeValue());
	}

	void Slider::OnPropertyChanged(PropertyBase& property)
	{
		RangeBase::OnPropertyChanged(property);

		if(property == OrientationProperty)
		{
			_track->SetOrientation(GetOrientation(), ValueSource::Inheritance);
		}
	}

	void Slider::BuildTemplate()
	{
		// Track
		_track = New<Track>();

		// RepeatButton
		_button = New<RepeatButton>();
		_button->SetInterval(100);
		_button->SetContent(_track);
		_button->Click += [this](Button& sender, EventArgs e)
		{
			float x, y;
			SDL_GetMouseState(&x, &y);

			auto bounds = GetBounds();
			auto value = GetOrientation() == Orientation::Horizontal
				? (GetMaxValue() - GetMinValue()) * (x - bounds.x) / bounds.w
				: (GetMaxValue() - GetMinValue()) * (1.0 - (y - bounds.y) / bounds.h);

			SetValue(value);
		};

		SetTemplate(_button);
	}
}