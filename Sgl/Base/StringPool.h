#pragma once

#include <unordered_set>
#include "Tools/StringUtils.h"

namespace Sgl
{
	class StringPool
	{
	public:
		StringPool() = default;
		const std::string* Create(std::string_view str);
	private:
		std::unordered_set<std::string, StringHash, std::equal_to<>> _map;
	};
}