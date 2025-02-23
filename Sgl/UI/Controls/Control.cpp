#include "Control.h"

namespace Sgl
{
	Control::Control(UIElement& parent) noexcept:
		Component(parent)
	{
		AddProperty<Color>(BorderColorProperty, Colors::Transparent);
		AddProperty<Thikness>(BorderThiknessProperty);
		AddProperty<FontFamily>(FontFamilyProperty);
		AddProperty<FontWeight>(FontWeightProperty, FontWeight::Normal);
		AddProperty<Color>(FontColorProperty, Colors::Black);
		AddProperty<uint16_t>(FontSizeProperty, DefaultFontSize);
	}
}