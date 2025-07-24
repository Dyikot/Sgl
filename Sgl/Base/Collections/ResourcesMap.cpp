#include "ResourcesMap.h"

namespace Sgl
{
	void ResourcesMap::Add(std::string_view key, Any item)
	{
		_resources[key] = item;
	}

	bool ResourcesMap::TryAdd(std::string_view key, Any item)
	{
		return _resources.try_emplace(key, std::move(item)).second;
	}

	void ResourcesMap::Remove(std::string_view key)
	{
		_resources.erase(key);
	}

	Any* ResourcesMap::TryGet(std::string_view key)
	{
		if(auto it = _resources.find(key); it != _resources.end())
		{
			return &it->second;
		}

		return nullptr;
	}

	bool ResourcesMap::Contatins(std::string_view key) const
	{
		return _resources.contains(key);
	}

	size_t ResourcesMap::Count() const
	{
		return _resources.size();
	}

	ResourcesMap::KeysView ResourcesMap::Keys() const
	{
		return std::views::keys(_resources);
	}

	ResourcesMap::ValuesView ResourcesMap::Values() const
	{
		return std::views::values(_resources);
	}

	Any& ResourcesMap::operator[](std::string_view key)
	{
		return _resources[key];
	}

	const Any& ResourcesMap::operator[](std::string_view key) const
	{
		return _resources.at(key);
	}
}
