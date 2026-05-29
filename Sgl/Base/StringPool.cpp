#include "StringPool.h"

namespace Sgl
{
	const std::string* StringPool::Create(std::string_view str)
	{
		if(auto it = _map.find(str); it != _map.end())
		{
			return it.operator->();
		}

		auto [it, _] = _map.emplace(str);
		return it.operator->();
	}
}

