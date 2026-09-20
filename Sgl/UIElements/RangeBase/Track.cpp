#include "Track.h"

namespace Sgl::UIElements
{
	Track::Track()
	{
		Name = "Track";
		SetBackground(Colors::White, ValueSource::Default);
		BuildTemplate();
	}

	void Track::SetFillRatio(float value, ValueSource source)
	{
		SetProperty(FillRatioProperty, _fillRatio, value, _fillRatioSource, source);
	}

	void Track::SetOrientation(Orientation value, ValueSource source)
	{
		SetProperty(OrientationProperty, _orientation, value, _orientationSource, source);
	}

	void Track::OnPropertyChanged(PropertyBase& property)
	{
		TemplatedUIElement::OnPropertyChanged(property);

		if(property == FillRatioProperty || property == OrientationProperty)
		{
			InvalidateArrange();
		}
	}

	void Track::ArrangeContent(FRect rect)
	{
		auto [width, height] = GetDesiredSize();	

		FRect contentRect = rect;

		if(_orientation == Orientation::Horizontal)
		{
			if(width == 0.0f)
			{
				width = rect.w;
			}

			contentRect.w = width * _fillRatio;
		}
		else
		{
			if(height == 0.0f)
			{
				height = rect.h;
			}

			contentRect.h = height * _fillRatio;
			contentRect.y = rect.y + height - contentRect.h;
		}

		TemplatedUIElement::ArrangeContent(contentRect);
	}

	void Track::BuildTemplate()
	{
		auto progress = New<UIElement>();
		progress->Name = "Progress";
		progress->SetBackground(Colors::Blue, ValueSource::Default);
		
		SetTemplate(progress);
	}
}