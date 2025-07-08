#pragma once

#include <unordered_map>
#include <string>
#include <ranges>
#include "../Object.h"

namespace Sgl
{
	class ResourcesMap
	{
	private:
		std::unordered_map<std::string, object> _resources;
	public:
		using KeysView = std::ranges::elements_view<std::ranges::ref_view<const std::unordered_map<std::string, object>>, 0ui64>;
		using ValuesView = std::ranges::elements_view<std::ranges::ref_view<const std::unordered_map<std::string, object>>, 1Ui64>;

		auto begin() { return _resources.begin(); }
		auto end() { return _resources.end(); }

		auto begin() const { return _resources.begin(); }
		auto end() const { return _resources.end(); }

		void Add(std::string key, object item);
		bool TryAdd(std::string key, object item);
		void Remove(const std::string& key);
		object* TryGet(const std::string& key);
		bool Contatins(const std::string& key) const;
		size_t Count() const;
		KeysView Keys() const;
		ValuesView Values() const;

		object& operator[](const std::string& key);
		const object& operator[](const std::string& key) const;
	};
}