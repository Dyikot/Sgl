#include "Border.h"
#include "../Application.h"
#include "../Layout/LayoutHelper.h"

namespace Sgl::UIElements
{
	Border::Border(Border&& other) noexcept:
		Decorator(std::move(other)),
		_borderWidth(other._borderWidth),
		_borderColor(other._borderColor)
	{
		Name = "Border";
	}

	void Border::SetBorderWidth(uint32_t value, ValueSource source)
	{
		if(SetProperty(BorderWidthProperty, _borderWidth, value, _borderWidthSource, source))
		{
			InvalidateMeasure();
		}
	}

	void Border::SetBorderColor(Color value, ValueSource source)
	{
		if(SetProperty(BorderColorProperty, _borderColor, value, _borderColorSource, source))
		{
			InvalidateRender();
		}
	}

	void Border::Render(RenderContext context)
	{
		Decorator::Render(context);
		
		if(_borderWidth == 0)
		{
			return;
		}

		float cornersRadius = GetCornersRadius();

		if(cornersRadius > 0.0f)
		{
			context.DrawRoundedRectangle(GetBounds(), cornersRadius, _borderWidth, _borderColor);
		}
		else
		{
			context.DrawRectangle(GetBounds(), _borderWidth, _borderColor);
		}
	}

	FSize Border::MeasureContent(FSize availableSize)
	{
		return MeasureChild(GetChild().Get(), availableSize, GetPadding().Inflate(_borderWidth));
	}

	void Border::ArrangeContent(FRect rect)
	{
		ArrangeChild(GetChild().Get(), rect, GetPadding().Inflate(_borderWidth));
	}
}

namespace Sgl
{
	ResourceSetter<UIElements::Border, Color>::ResourceSetter(
		BorderColorProperty& property,
		ResourceKey key):
		Setter(property),
		_key(std::move(key))
	{}

	void ResourceSetter<UIElements::Border, Color>::Apply(
		StyleableElement& target,
		ValueSource valueSource) const
	{
		auto& property = static_cast<BorderColorProperty&>(GetProperty());
		property.InvokeSetter(
			static_cast<UIElements::Border&>(target),
			App->Resources.GetColor(_key.Value),
			valueSource
		);
	}
}