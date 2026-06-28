#pragma once

#include "ToggleButton.h"

namespace Sgl::UIElements
{
	class SwitchButon : public ToggleButton
	{
	public:
		SwitchButon();
		SwitchButon(SwitchButon&&) noexcept = default;
	};
}