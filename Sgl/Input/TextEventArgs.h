#pragma once

#include <string>

namespace Sgl
{
	struct TextEditingEventArgs
	{
		std::string Text;
		size_t SelectionStart;
		size_t SelectionLength;
	};

	struct TextInputEventArgs
	{
		std::string Text;
	};
}