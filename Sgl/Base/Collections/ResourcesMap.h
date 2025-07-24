#pragma once

#include <unordered_map>
#include <string_view>
#include <ranges>
#include "../Any.h"

namespace Sgl
{
	class ResourcesMap
	{
	private:
		std::unordered_map<std::string_view, Any> _resources;
	public:
		using KeysView = std::ranges::elements_view<std::ranges::ref_view<const std::unordered_map<std::string_view, Any>>, 0ui64>;
		using ValuesView = std::ranges::elements_view<std::ranges::ref_view<const std::unordered_map<std::string_view, Any>>, 1Ui64>;

		auto begin() { return _resources.begin(); }
		auto end() { return _resources.end(); }

		auto begin() const { return _resources.begin(); }
		auto end() const { return _resources.end(); }

		void Add(std::string_view key, Any item);
		bool TryAdd(std::string_view key, Any item);
		void Remove(std::string_view key);
		Any* TryGet(std::string_view key);
		bool Contatins(std::string_view key) const;
		size_t Count() const;
		KeysView Keys() const;
		ValuesView Values() const;

		Any& operator[](std::string_view key);
		const Any& operator[](std::string_view key) const;
	};
}