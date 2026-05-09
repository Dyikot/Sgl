#pragma once

#include "ToggleButton.h"

namespace Sgl::UIElements
{
	class CheckBox : public ToggleButton
	{
	public:
		CheckBox();
		CheckBox(CheckBox&&) noexcept = default;
	};
}