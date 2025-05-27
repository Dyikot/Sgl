#pragma once

#include <unordered_map>
#include <string_view>
#include "Any.h"

namespace Sgl
{
	using ResourcesMap = std::unordered_map<std::string_view, Any>;
}