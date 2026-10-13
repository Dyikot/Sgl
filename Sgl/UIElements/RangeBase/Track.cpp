#include "Track.h"
#include "../../Styling/StyleContext.h"

namespace Sgl::UIElements
{
	Track::Track()
	{
		SetName("Track");
		BuildTemplate();
	}

	void Track::SetFillRatio(float value)
	{
		SetProperty(FillRatioProperty, _fillRatio, value, _fillRatioSource);
	}

	void Track::SetOrientation(Orientation value)
	{
		SetProperty(OrientationProperty, _orientation, value, _orientationSource);
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

	Ref<Style> Track::CreateDefaultStyle() const
	{
		return New<TrackStyle>();
	}

	void Track::BuildTemplate()
	{
		auto progress = New<UIElement>();
		progress->SetName("Progress");
		progress->SetStyle(New<TrackProgressStyle>());

		SetTemplate(progress);
	}

	void TrackStyle::OnBase(StyleContext& context)
	{
		context.Set(Track::BackgroundProperty, Colors::Grey_1);
	}

	void TrackProgressStyle::OnBase(StyleContext& context)
	{
		context.Set(Track::BackgroundProperty, Colors::Blue);
	}
}