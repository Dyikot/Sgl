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

	void Control::SetBackgound(const Fill& value)
	{
		SetProperty(BackgroundProperty, value);
		OnPropertyChanged(BackgroundProperty);
	}

	void Control::SetBorderColor(Color value)
	{
		SetProperty(BorderColorProperty, value);
		OnPropertyChanged(BorderColorProperty);
	}

	void Control::SetFontColor(Color value)
	{
		SetProperty(FontColorProperty, value);
		OnPropertyChanged(FontColorProperty);
	}

	void Control::SetBorderThikness(const Thikness & value)
	{
		SetProperty(BorderThiknessProperty, value);
		OnPropertyChanged(BorderThiknessProperty);
	}

	void Control::SetFontFamily(const FontFamily & value)
	{
		SetProperty(FontFamilyProperty, value);
		OnPropertyChanged(FontFamilyProperty);
	}

	void Control::SetFontSize(uint16_t value)
	{
		SetProperty(FontSizeProperty, value);
		OnPropertyChanged(FontSizeProperty);
	}

	void Control::SetFontWeight(FontWeight value)
	{
		SetProperty(FontWeightProperty, value);
		OnPropertyChanged(FontWeightProperty);
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