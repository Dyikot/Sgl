#include "Control.h"

namespace Sgl
{
	Control::Control():
		Control(SDL_FPoint())
	{}

	Control::Control(SDL_FPoint position) noexcept:
		Component(position),
		_isEventsInitialized(InitializeEvents()),
		MouseDoubleClick(GetEventValue<MouseEventHandler>(MouseDoubleClickEvent))
	{
		AddProperty<Fill>(BackgroundProperty, Colors::Transparent);
		AddProperty<Color>(BorderColorProperty, Colors::Transparent);
		AddProperty<Thikness>(BorderThiknessProperty);
		AddProperty<FontFamily>(FontFamilyProperty);
		AddProperty<FontWeight>(FontWeightProperty, FontWeight::Normal);
		AddProperty<Color>(FontColorProperty, Colors::Black);
		AddProperty<uint16_t>(FontSizeProperty, DefaultFontSize);
	}

	void Control::OnMouseDoubleClick(const MouseButtonEventArgs& e)
	{
		if(MouseDoubleClick) 
		{
			MouseDoubleClick(this, e);
		}
	}

	bool Control::InitializeEvents()
	{
		AddEvent<MouseEventHandler>(MouseDoubleClickEvent);

		return true;
	}
}