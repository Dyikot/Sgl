#pragma once

#include <unordered_map>
#include <string_view>
#include "../Any.h"

namespace Sgl
{
	class ResourcesMap
	{
	private:
		std::unordered_map<std::string_view, Any> _resources;
	public:
		void Add(std::string_view key, Any item);
		void AddTexture(std::string_view key, std::string_view path);
		void Remove(std::string_view key);
		Any& Get(std::string_view key);
		Any* TryGet(std::string_view key);
		bool Contatins(std::string_view key) const;

		Any& operator[](std::string_view key);
	};
}