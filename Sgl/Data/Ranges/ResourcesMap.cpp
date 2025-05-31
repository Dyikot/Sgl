#include "ResourcesMap.h"
#include "../../Visual/Renderer.h"

namespace Sgl
{
	void ResourcesMap::Add(std::string_view key, Any value)
	{
		_resources.emplace(key, std::move(value));
	}

	void ResourcesMap::AddTexture(std::string_view key, std::string_view path)
	{
		Renderer r;
		Add(key, r.LoadTexture(path));
	}

	void ResourcesMap::Remove(std::string_view key)
	{
		_resources.erase(key);
	}

	Any& ResourcesMap::Get(std::string_view key)
	{
		return _resources[key];
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

	Any& ResourcesMap::operator[](std::string_view key)
	{
		return _resources[key];
	}
}
