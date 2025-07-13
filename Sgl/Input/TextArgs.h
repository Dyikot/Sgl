#pragma once

#include <string>
#include "../Base/Events/EventArgs.h"

namespace Sgl
{
	struct TextChangedEventArgs: EventArgs
	{
		std::string Text;
		size_t SelectionStart;
		size_t SelectionLength;
	};

	struct TextInputEventArgs: EventArgs
	{
		std::string Text;
	};
}