#pragma once

#include <string>

namespace Sgl
{
	struct TextEditingEventArgs
	{
		std::string Text;
		int SelectionStart;
		int SelectionLength;
	};

	struct TextInputEventArgs
	{
		std::string Text;
	};
}