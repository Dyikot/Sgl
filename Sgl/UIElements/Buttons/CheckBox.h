#pragma once

#include "ToggleButton.h"

namespace Sgl::UIElements
{
	class CheckBox : public ToggleButton
	{
	public:
		CheckBox();
	protected:
		Ref<Style> CreateDefaultStyle() const override;
	};

	class CheckBoxStyle : public ToggleButtonStyle
	{
	public:
		void OnBase(StyleContext& context) override;
	protected:
		void OnChecked(StyleContext& context) override;
	};
}