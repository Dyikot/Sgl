#include "ProgressBar.h"
#include "Track.h"

namespace Sgl::UIElements
{
	ProgressBar::ProgressBar()
	{
		Name = "ProgressBar";
		BuildTemplate();
	}

	ProgressBar::ProgressBar(ProgressBar&& other) noexcept:
		RangeBase(std::move(other)),
		_track(std::move(other._track))
	{}

	void ProgressBar::OnValueChanged(float value)
	{
		RangeBase::OnValueChanged(value);
		_track->SetFillRatio(GetRelativeValue());
	}

	void ProgressBar::OnPropertyChanged(PropertyBase& property)
	{
		RangeBase::OnPropertyChanged(property);

		if(property == OrientationProperty)
		{
			_track->SetOrientation(GetOrientation(), ValueSource::Inheritance);
		}
	}

	void ProgressBar::BuildTemplate()
	{
		_track = New<Track>();
		SetTemplate(_track);
	}
}