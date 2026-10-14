#include "ProgressBar.h"
#include "Track.h"

namespace Sgl::UIElements
{
	ProgressBar::ProgressBar()
	{
		SetName("ProgressBar");
		BuildTemplate();
	}

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
			ValueSourceScope scope(_track, ValueSource::Inheritance);
			_track->SetOrientation(GetOrientation());
		}
	}

	void ProgressBar::BuildTemplate()
	{
		_track = New<Track>();
		AddChild(_track);
	}
}