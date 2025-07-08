#pragma once

#include <string>
#include "../Base/Primitives.h"
#include "../Base/Time/TimeSpan.h"

namespace Sgl
{
	struct ToolTip
	{
		std::string Value;
		FPoint Position;
		TimeSpan ShowDelay;
	};
}