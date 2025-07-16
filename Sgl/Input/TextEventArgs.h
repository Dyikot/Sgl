#pragma once

#include <string>
#include "../Base/EventArgs.h"

namespace Sgl
{
	struct TextEditingEventArgs: EventArgs
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