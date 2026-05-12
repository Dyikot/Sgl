#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

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
		std::vector<std::string> _pool;
		std::unordered_map<std::string_view, Handle> _map;
	};
}