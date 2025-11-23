#pragma once

#include <string>
#include <string_view>

namespace Sgl
{
	class Clipboard
	{
	public:
		static void SetText(std::string_view text);
		static std::string GetText();
		static void Clear();
		static bool IsEmpty();
	};
}