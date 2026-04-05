#pragma once

#include "ToggleButton.h"

namespace Sgl::UIElements
{
	class CheckBox : public ToggleButton
	{
	public:
		CheckBox();
		CheckBox(const CheckBox&) = delete;
		CheckBox(CheckBox&&) noexcept = default;
	};
}