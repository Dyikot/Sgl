#pragma once

#include <unordered_map>
#include <string>
#include <ranges>
#include "../Any.h"

namespace Sgl
{
	class ResourcesMap
	{
	private:
		std::unordered_map<std::string, Any> _resources;
	public:
		using KeysView = std::ranges::elements_view<std::ranges::ref_view<const std::unordered_map<std::string, Any>>, 0ui64>;
		using ValuesView = std::ranges::elements_view<std::ranges::ref_view<const std::unordered_map<std::string, Any>>, 1Ui64>;

		auto begin() { return _resources.begin(); }
		auto end() { return _resources.end(); }

		auto begin() const { return _resources.begin(); }
		auto end() const { return _resources.end(); }

		void Add(std::string key, Any item);
		bool TryAdd(std::string key, Any item);
		void Remove(const std::string& key);
		Any* TryGet(const std::string& key);
		bool Contatins(const std::string& key) const;
		size_t Count() const;
		KeysView Keys() const;
		ValuesView Values() const;

		Any& operator[](const std::string& key);
		const Any& operator[](const std::string& key) const;
	};
}