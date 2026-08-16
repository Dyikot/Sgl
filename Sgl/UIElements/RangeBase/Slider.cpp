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
		_button->SetInterval(25, ValueSource::Default);
		_button->SetContent(_track);
		_button->Click += [this](Button& sender, MouseClickEventArgs& e)
		{
			auto value = ValueAtPosition(e.X, e.Y);
			SetValue(value);
		};

		SetTemplate(_button);
	}

	float Slider::ValueAtPosition(float x, float y) const
	{
		auto bounds = GetBounds();
		return GetOrientation() == Orientation::Horizontal
			? (GetMaxValue() - GetMinValue()) * (x - bounds.x) / bounds.w
			: (GetMaxValue() - GetMinValue()) * (1.0f - (y - bounds.y) / bounds.h);
	}
}