#include "Border.h"

namespace Sgl::UIElements
{
	Border::Border():
		ContentUIElement(),
		_borderWidth(BorderWidthProperty.DefaultValue),
		_borderColor(BorderColorProperty.DefaultValue)
	{}

	Border::Border(const Border& other):
		_borderWidth(other._borderWidth),
		_borderColor(other._borderColor)
	{}

	Border::Border(Border&& other) noexcept:
		_borderWidth(other._borderWidth),
		_borderColor(other._borderColor)
	{}

	void Border::Render(RenderContext context) const
	{
		auto backgroundColor = GetBackgroundColor();

		if(!backgroundColor.IsTransparent())
		{
			context.DrawFillRectangle(_bounds, backgroundColor);
		}

		if(!_borderColor.IsTransparent() && _borderWidth > 0)
		{
			context.DrawRectange(_bounds, _borderColor);
		}

		ContentUIElement::Render(context);
	}
}

