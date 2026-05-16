#pragma once

#include <unordered_map>
#include "Tools/StringUtils.h"

namespace Sgl
{
	class StringPool
	{
	public:
		using Handle = uint32_t;
	public:
		StringPool() = default;
		std::string_view Get(Handle handle);
		Handle Create(std::string_view str);
	private:
		std::vector<std::string_view> _pool;
		std::unordered_map<std::string, Handle, StringHash, std::equal_to<>> _map;
	};
}