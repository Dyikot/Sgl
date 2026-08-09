#include "Slider.h"
#include "Track.h"
#include "../Buttons/RepeatButton.h"

namespace Sgl::UIElements
{
	Slider::Slider()
	{
		Name = "Slider";
		BuildTemplate();
	}

	Slider::Slider(Slider&& other) noexcept:
		RangeBase(std::move(other)),
		_button(std::move(other._button)),
		_track(std::move(other._track))
	{}

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
		_track = New<Track>();

		_button = New<RepeatButton>();
		_button->SetInterval(25);
		_button->SetContent(_track);
		_button->Click += [this](Button& sender, EventArgs e)
		{
			float x, y;
			SDL_GetMouseState(&x, &y);

			auto bounds = GetBounds();
			auto value = GetOrientation() == Orientation::Horizontal
				? (GetMaxValue() - GetMinValue()) * (x - bounds.x) / bounds.w
				: (GetMaxValue() - GetMinValue()) * (1.0f - (y - bounds.y) / bounds.h);

			SetValue(value);
		};

		SetTemplate(_button);
	}
}