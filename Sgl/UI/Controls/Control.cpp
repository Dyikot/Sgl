#include "Control.h"

namespace Sgl
{
	Control::Control(SDL_FPoint position) noexcept:
		Component(position)
	{
		AddProperty<Color>(BorderColorProperty, Colors::Transparent);
		AddProperty<Thikness>(BorderThiknessProperty);
		AddProperty<FontFamily>(FontFamilyProperty);
		AddProperty<FontWeight>(FontWeightProperty, FontWeight::Normal);
		AddProperty<Color>(FontColorProperty, Colors::Black);
		AddProperty<uint16_t>(FontSizeProperty, DefaultFontSize);
	}
}