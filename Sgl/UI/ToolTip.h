#pragma once

#include <string>
#include "../Render/Primitives.h"
#include "../Tools/Time/TimeSpan.h"

namespace Sgl
{
	struct ToolTip
	{
		std::string Value;
		FPoint Position;
		TimeSpan ShowDelay;
	};
}