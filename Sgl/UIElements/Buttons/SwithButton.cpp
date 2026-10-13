#include "SwithButton.h"
#include "../../Styling/StyleContext.h"

namespace Sgl::UIElements
{
	SwitchButon::SwitchButon()
	{
		SetName("SwitchButon");
	}

	Ref<Style> SwitchButon::CreateDefaultStyle() const
	{
		return New<SwitchButonStyle>();
	}

	void SwitchButonStyle::OnBase(StyleContext& context)
	{
		ToggleButtonStyle::OnBase(context);
		context.Set(Button::WidthProperty, 48);
		context.Set(Button::HeightProperty, 24);
		context.Set(Button::VerticalAlignmentProperty, VerticalAlignment::Top);
		context.Set(Button::HorizontalAlignmentProperty, HorizontalAlignment::Left);
		context.Set(Button::BackgroundProperty, ImageSource(AssetId::SwitchButtonOff));
	}

	void SwitchButonStyle::OnChecked(StyleContext& context)
	{
		context.Set(Button::BackgroundProperty, ImageSource(AssetId::SwitchButtonOn));
	}
}

