#include "StringPool.h"
#include "Exceptions.h"

namespace Sgl
{
	std::string_view StringPool::Get(Handle handle)
	{
		size_t index = static_cast<size_t>(handle);
		if(index >= _pool.size())
		{
			throw Exception("String not found!");
		}

		return _pool[index];
	}

	StringPool::Handle StringPool::Create(std::string_view str)
	{
		if(auto it = _map.find(str); it != _map.end())
		{
			return it->second;
		}

		auto handle = _pool.size();
		auto [it, _] = _map.emplace(str, handle);
		auto& stored_value = _pool.emplace_back(it->first);
		return handle;
	}
}

