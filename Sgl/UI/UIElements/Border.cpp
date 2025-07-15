#include "Border.h"

namespace Sgl
{
	Border::Border():
		ContentUIElement(),
		BorderWidth(1),
		BorderColor(Colors::Black)
	{}

	Border::Border(const Border& other):
		BorderWidth(other.BorderWidth),
		BorderColor(other.BorderColor)
	{}

	Border::Border(Border&& other) noexcept:
		BorderWidth(other.BorderWidth),
		BorderColor(other.BorderColor)
	{}

	void Border::OnRender(RenderContext context) const
	{
		Color backgroundColor = BackgroundColor;
		Color borderColor = BorderColor;

		if(!backgroundColor.IsTransparent())
		{
			context.DrawFillRectangle(_bounds, backgroundColor);
		}

		if(!borderColor.IsTransparent() && BorderWidth > 0)
		{
			context.DrawRectange(_bounds, borderColor);
		}

		ContentUIElement::OnRender(context);
	}
}

