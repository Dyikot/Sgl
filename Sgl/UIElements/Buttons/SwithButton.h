#pragma once

#include "ToggleButton.h"

namespace Sgl::UIElements
{
	class SwitchButon : public ToggleButton
	{
	public:
		SwitchButon();
	protected:
		Ref<Style> CreateDefaultStyle() const override;
	};

	class SwitchButtonStyle : public ToggleButtonStyle
	{
	public:
		void OnBase(StyleContext& context) override;
	protected:
		void OnChecked(StyleContext& context) override;
	};
}