#include "Border.h"
#include "../Layout/LayoutHelper.h"

namespace Sgl::UIElements
{
	Border::Border()
	{
		SetName("Border");
	}

	void Border::SetBorderWidth(uint32_t value, ValueSource source)
	{
		SetProperty(BorderWidthProperty, _borderWidth, value, _borderWidthSource, source);
	}

	uint32_t Border::GetBorderWidth() const
	{
		return GetProperty(BorderWidthProperty, _borderWidth);
	}

	void Border::SetBorderColor(Color value, ValueSource source)
	{
		SetProperty(BorderColorProperty, _borderColor, value, _borderColorSource, source);
	}

	Color Border::GetBorderColor() const
	{
		return GetProperty(BorderColorProperty, _borderColor);
	}

	void Border::OnRender(RenderContext& context)
	{		
		float borderWidth = GetBorderWidth();

		if(borderWidth == 0.0f)
		{
			return;
		}

		float cornersRadius = GetCornersRadius();
		Color borderColor = GetBorderColor();

		if(cornersRadius > 0.0f)
		{
			context.DrawRoundedRectangle(GetBounds(), cornersRadius, borderWidth, borderColor);
		}
		else
		{
			context.DrawRectangle(GetBounds(), borderWidth, borderColor);
		}
	}

	void Border::OnPropertyChanged(PropertyBase& property)
	{
		Decorator::OnPropertyChanged(property);

		if(property == BorderColorProperty)
		{
			InvalidateRender();
		}
		else if(property == BorderWidthProperty)
		{
			InvalidateMeasure();
		}
	}

	FSize Border::MeasureContent(FSize availableSize)
	{
		return MeasureChild(GetChild().Get(), availableSize, GetPadding().Inflate(GetBorderWidth()));
	}

	void Border::ArrangeContent(FRect rect)
	{
		ArrangeChild(GetChild().Get(), rect, GetPadding().Inflate(GetBorderWidth()));
	}
}