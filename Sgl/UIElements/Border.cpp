#include "Border.h"
#include "../Layout/LayoutHelper.h"

namespace Sgl::UIElements
{
	Border::Border()
	{
		SetName("Border");
	}

	void Border::SetChild(const Ref<UIElement>& value)
	{
		auto source = GetValueSource();

		if(_childSource > source)
		{
			return;
		}

		if(_child == value)
		{
			_childSource = source;
			return;
		}

		if(_child)
		{
			RemoveChild(_child);
		}

		_child = value;
		_childSource = source;

		if(_child)
		{
			AddChild(_child);
		}

		OnPropertyChanged(ChildProperty);
	}

	void Border::SetPadding(Thickness value)
	{
		SetProperty(PaddingProperty, _padding, value, _paddingSource);
	}

	void Border::SetBorderWidth(uint32_t value)
	{
		SetProperty(BorderWidthProperty, _borderWidth, value, _borderWidthSource);
	}

	uint32_t Border::GetBorderWidth() const
	{
		return GetProperty(BorderWidthProperty, _borderWidth);
	}

	void Border::SetBorderColor(Color value)
	{
		SetProperty(BorderColorProperty, _borderColor, value, _borderColorSource);
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
		UIElement::OnPropertyChanged(property);

		if(property == BorderColorProperty)
		{
			InvalidateRender();
		}
		else if(property == BorderWidthProperty || 
				property == ChildProperty || 
				property == PaddingProperty)
		{
			InvalidateMeasure();
		}
	}

	FSize Border::MeasureContent(FSize availableSize)
	{
		return MeasureChild(_child.Get(), availableSize, _padding.Inflate(GetBorderWidth()));
	}

	void Border::ArrangeContent(FRect rect)
	{
		ArrangeChild(_child.Get(), rect, _padding.Inflate(GetBorderWidth()));
	}
}