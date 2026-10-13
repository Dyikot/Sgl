#include "CheckBox.h"
#include "../../Styling/StyleContext.h"

namespace Sgl::UIElements
{
	CheckBox::CheckBox()
	{
		SetName("CheckBox");
	}

	Ref<Style> CheckBox::CreateDefaultStyle() const
	{
		return New<CheckBoxStyle>();
	}

	void CheckBoxStyle::OnBase(StyleContext& context)
	{
		ToggleButtonStyle::OnBase(context);
		context.Set(Button::WidthProperty, 24);
		context.Set(Button::HeightProperty, 24);
		context.Set(Button::VerticalAlignmentProperty, VerticalAlignment::Top);
		context.Set(Button::HorizontalAlignmentProperty, HorizontalAlignment::Left);
		context.Set(Button::BackgroundProperty, ImageSource(AssetId::CheckBoxOutline));
	}

	void CheckBoxStyle::OnChecked(StyleContext& context)
	{
		context.Set(Button::BackgroundProperty, ImageSource(AssetId::CheckBox));
	}
}