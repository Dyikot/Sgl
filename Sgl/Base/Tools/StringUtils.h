#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace Sgl
{
	std::vector<std::string> SplitString(std::string_view str, char delimiter);
}