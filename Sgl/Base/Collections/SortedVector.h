#pragma once

#include <vector>
#include <ranges>
#include <optional>
#include <algorithm>
#include "../Delegate.h"

namespace Sgl
{
	template<typename T, typename TComparer = std::less<T>>
	class SortedVector
	{
	protected:
		TComparer _comparer;
		std::vector<T> _items;
	public:
		SortedVector() = default;

		SortedVector(const SortedVector& other):
			_comparer(other._comparer),
			_items(other._items)
		{}

		SortedVector(SortedVector&& other) noexcept:
			_comparer(std::move(other._comparer)),
			_items(std::move(other._items))
		{}

		SortedVector(std::initializer_list<T> init):
			_items(init)
		{
			std::ranges::sort(_items, _comparer);
		}

		template<std::ranges::range TRange>
			requires std::same_as<std::ranges::range_value_t<TRange>, T>
		SortedVector(TRange&& range):
			_items(std::ranges::begin(range), std::ranges::end(range))
		{
			std::ranges::sort(_items, _comparer);
		}

		~SortedVector() = default;

		auto begin() { return _items.begin(); }
		auto end() { return _items.end(); }

		auto begin() const { return _items.begin(); }
		auto end() const { return _items.end(); }

		void Add(const T& item)
		{
			_items.insert(LowerBound(item), item);
		}

		void Add(T&& item)
		{
			_items.insert(LowerBound(item), std::forward<T>(item));
		}

		template<std::ranges::range TRange>
			requires std::same_as<std::ranges::range_value_t<TRange>, T>
		void AddRange(TRange&& range)
		{
			_items.insert(_items.end(), std::ranges::begin(range), std::ranges::end(range));
			std::ranges::sort(_items, _comparer);
		}

		size_t Count() const noexcept
		{
			return _items.size();
		}

		size_t Capacity() const noexcept
		{
			return _items.capacity();
		}

		void Clear() noexcept
		{
			_items.clear();
		}

		bool Contains(const T& item) const
		{
			return std::ranges::binary_search(_items, item, _comparer);
		}

		const T& ElementAt(size_t index) const
		{
			return _items.at(index);
		}

		auto LowerBound(const T& item) const
		{
			return std::ranges::lower_bound(_items, item, _comparer);
		}

		auto UpperBound(const T& item) const
		{
			return std::ranges::upper_bound(_items, item, _comparer);
		}

		std::optional<size_t> Find(const T& item) const
		{
			auto it = LowerBound(item);

			if(it != _items.end() && !_comparer(item, *it))
			{
				return std::ranges::distance(_items.begin(), it);
			}

			return std::nullopt;
		}

		void Remove(const T& item)
		{
			auto it = LowerBound(item);

			if(it != _items.end() && !_comparer(item, *it))
			{
				_items.erase(it);
			}
		}

		SortedVector& operator=(std::initializer_list<T> items)
		{
			_items = items;
			std::ranges::sort(_items, _comparer);
			return *this;
		}
	};
}