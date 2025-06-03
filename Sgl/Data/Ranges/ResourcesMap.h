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
		auto begin() { return _resources.begin(); }
		auto begin() const { return _resources.begin(); }

		auto end() { return _resources.end(); }
		auto end() const { return _resources.end(); }

		void Add(std::string_view key, Any item);
		void AddTexture(std::string_view key, std::string_view path);
		void Remove(std::string_view key);
		Any& Get(std::string_view key);
		Nullable<Any> TryGet(std::string_view key);
		bool Contatins(std::string_view key) const;

		Any& operator[](std::string_view key);
	};
}