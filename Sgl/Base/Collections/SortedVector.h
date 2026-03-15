#pragma once

#include <vector>
#include <ranges>
#include <algorithm>

namespace Sgl
{
	template<typename T, typename TComparer = std::less<T>>
	class SortedVector
	{
	public:
		SortedVector() = default;

		explicit SortedVector(size_t capacity)
		{
			_items.reserve(capacity);
		}

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

		SortedVector(const SortedVector& other):
			_items(other._items),
			_comparer(other._comparer)
		{}

		SortedVector(SortedVector&& other) noexcept:
			_items(std::move(other._items)),
			_comparer(std::move(other._comparer))
		{}		

		auto begin() const { return _items.begin(); }
		auto end() const { return _items.end(); }

		auto rbegin() const { return _items.rbegin(); }
		auto rend() const { return _items.rend(); }

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
			if constexpr(std::ranges::sized_range<TRange>)
			{
				_items.reserve(_items.size() + std::ranges::size(range));
			}

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

		bool IsEmpty() const noexcept
		{
			return _items.empty();
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

		void SetElementAt(size_t index, const T& value)
		{
			_items[index] = value;
			std::ranges::sort(_items, _comparer);
		}

		void SetElementAt(size_t index, T&& value)
		{
			_items[index] = std::forward<T>(value);
			std::ranges::sort(_items, _comparer);
		}

		auto LowerBound(const T& item) const
		{
			return std::ranges::lower_bound(_items, item, _comparer);
		}

		auto UpperBound(const T& item) const
		{
			return std::ranges::upper_bound(_items, item, _comparer);
		}

		int Find(const T& item) const
		{
			auto it = LowerBound(item);

			if(it != _items.end() && !_comparer(item, *it))
			{
				return static_cast<int>(it - _items.begin());
			}

			return -1;
		}

		void Remove(const T& item)
		{
			auto it = LowerBound(item);

			if(it != _items.end() && !_comparer(item, *it))
			{
				_items.erase(it);
			}
		}

		void RemoveAt(size_t index)
		{
			_items.erase(_items.begin() + index);
		}

		const T& operator[](size_t index) const
		{
			return _items[index]; 
		}

		SortedVector& operator=(std::initializer_list<T> items)
		{
			_items = items;
			std::ranges::sort(_items, _comparer);
			return *this;
		}

		SortedVector& operator=(const SortedVector&) = default;
		SortedVector& operator=(SortedVector&&) noexcept = default;
	protected:
		std::vector<T> _items;
		TComparer _comparer;
	};
}