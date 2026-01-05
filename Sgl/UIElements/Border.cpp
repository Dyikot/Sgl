#include "Border.h"

namespace Sgl::UIElements
{
	Border::Border(const Border& other):
		ContentUIElement(other),
		_borderWidth(other._borderWidth),
		_borderColor(other._borderColor)
	{}

	Border::Border(Border&& other) noexcept:
		ContentUIElement(std::move(other)),
		_borderWidth(other._borderWidth),
		_borderColor(other._borderColor)
	{}

	void Border::SetBorderWidth(size_t value)
	{
		SetBorderWidth(value, ValueSource::Local);
	}

	void Border::SetBorderWidth(size_t value, ValueSource source)
	{
		if(SetProperty(BorderWidthProperty, _borderWidth, value, _borderWidthSource, source))
		{
			InvalidateMeasure();
		}
	}

	void Border::SetBorderColor(Color value)
	{
		SetBorderColor(value, ValueSource::Local);
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
		RenderBackground(context);

		if(!_borderColor.IsTransparent() && _borderWidth > 0)
		{
			context.DrawRectange(_bounds, _borderColor);
		}

		ContentUIElement::Render(context);
	}
}

