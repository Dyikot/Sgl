#include "ProgressBar.h"

namespace Sgl::UIElements
{
	ProgressBar::ProgressBar()
	{
		SetBackground(Colors::LightGray, ValueSource::Default);
	}

	ProgressBar::ProgressBar(ProgressBar&& other) noexcept:
		RangeBase(std::move(other)),
		_progressColor(other._progressColor)
	{}

	void ProgressBar::SetProgressColor(Color value, ValueSource source)
	{
		if(SetProperty(ProgressColorProperty, _progressColor, value, _progressColorSource, source))
		{
			InvalidateRender();
		}
	}

	void ProgressBar::Render(RenderContext context)
	{
		UIElement::Render(context);

		auto [x, y, w, h] = GetBounds();

		auto orientation = GetOrientation();
		float relativeValue = GetRelativeValue();
		float cornersRadius = GetCornersRadius();

		FRect progress = orientation == Orientation::Horizontal
			? FRect(x, y, w * relativeValue, h)
			: FRect(x, y + h - h * relativeValue, w, h * relativeValue);

		if(cornersRadius > 0.0f)
		{
			if(orientation == Orientation::Horizontal && progress.w < cornersRadius)
			{
				progress.y = y + cornersRadius - progress.w;
				progress.h -= 2 * (cornersRadius - progress.w);
			}
			else if(orientation == Orientation::Vertical && progress.h < cornersRadius)
			{
				progress.x = x + cornersRadius - progress.h;
				progress.w -= 2 * (cornersRadius - progress.h);
			}

			context.DrawRectangleFill(progress, cornersRadius, _progressColor);
		}
		else
		{
			context.DrawRectangleFill(progress, _progressColor);
		}		
	}
}