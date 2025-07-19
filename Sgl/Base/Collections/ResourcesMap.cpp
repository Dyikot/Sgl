#include "ResourcesMap.h"

namespace Sgl
{
	void ResourcesMap::Add(std::string key, Any item)
	{
		_resources[std::move(key)] = item;
	}

	bool ResourcesMap::TryAdd(std::string key, Any item)
	{
		return _resources.try_emplace(std::move(key), std::move(item)).second;
	}

	void ResourcesMap::Remove(const std::string& key)
	{
		_resources.erase(key);
	}

	Any* ResourcesMap::TryGet(const std::string& key)
	{
		if(auto it = _resources.find(key); it != _resources.end())
		{
			return &it->second;
		}

		return nullptr;
	}

	bool ResourcesMap::Contatins(const std::string& key) const
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

	Any& ResourcesMap::operator[](const std::string& key)
	{
		return _resources[key];
	}

	const Any& ResourcesMap::operator[](const std::string& key) const
	{
		return _resources.at(key);
	}
}
