#pragma once
#include <vector>
#include <ranges>
#include <algorithm>

namespace Sgl
{
	//! @brief Represents a vector that automatically keeps its elements sorted
	//! @tparam T The type of elements in the vector
	//! @tparam TComparer The comparison function object type
	template<typename T, typename TComparer = std::less<T>>
	class SortedVector
	{
	public:
		//! @brief Default constructor
		SortedVector() = default;

		//! @brief Initializes the vector with a specific capacity
		//! @param capacity The initial capacity of the vector
		explicit SortedVector(size_t capacity)
		{
			_items.reserve(capacity);
		}

		//! @brief Initializes the vector with initial elements
		//! @param init The initializer list of elements
		SortedVector(std::initializer_list<T> init):
			_items(init)
		{
			std::ranges::sort(_items, _comparer);
		}

		//! @brief Copy constructor
		SortedVector(const SortedVector& other):
			_items(other._items),
			_comparer(other._comparer)
		{}

		//! @brief Move constructor
		SortedVector(SortedVector&& other) noexcept:
			_items(std::move(other._items)),
			_comparer(std::move(other._comparer))
		{}

		//! @brief Returns an iterator to the beginning of the vector
		auto begin() const { return _items.begin(); }

		//! @brief Returns an iterator to the end of the vector
		auto end() const { return _items.end(); }

		//! @brief Returns a reverse iterator to the beginning of the vector
		auto rbegin() const { return _items.rbegin(); }

		//! @brief Returns a reverse iterator to the end of the vector
		auto rend() const { return _items.rend(); }

		//! @brief Adds an item to the vector in sorted order
		//! @param item Lvalue reference to the item to add
		void Add(const T& item)
		{
			_items.insert(LowerBound(item), item);
		}

		//! @brief Adds an item to the vector in sorted order
		//! @param item Rvalue reference to the item to add
		void Add(T&& item)
		{
			_items.insert(LowerBound(item), std::move(item));
		}

		//! @brief Adds a range of elements to the vector and re-sorts it
		//! @param range The range of elements to add
		void AddRange(std::ranges::range auto&& range)
		{
			if constexpr(std::ranges::sized_range<decltype(range)>)
			{
				_items.reserve(_items.size() + std::ranges::size(range));
			}
			_items.insert(_items.end(), std::ranges::begin(range), std::ranges::end(range));
			std::ranges::sort(_items, _comparer);
		}

		//! @brief Gets the number of elements
		size_t Count() const noexcept
		{
			return _items.size();
		}

		//! @brief Gets the capacity
		size_t Capacity() const noexcept
		{
			return _items.capacity();
		}

		//! @brief Returns true if the vector is empty, false otherwise
		bool IsEmpty() const noexcept
		{
			return _items.empty();
		}

		//! @brief Erases all elements
		void Clear() noexcept
		{
			_items.clear();
		}

		//! @brief Checks if an element is in the vector
		//! @param item The element to check
		//! @return True if the element is in the vector, false otherwise
		bool Contains(const T& item) const
		{
			return std::ranges::binary_search(_items, item, _comparer);
		}

		//! @brief Returns a const reference to the element at the specified index
		//! @param index The zero-based index of the element to get
		const T& ElementAt(size_t index) const
		{
			return _items.at(index);
		}

		//! @brief Replaces the element at the specified index with a new value and re-sorts the vector
		//! @param index The zero-based index of the element to replace
		//! @param value The new value for the element
		void SetElementAt(size_t index, const T& value)
		{
			_items[index] = value;
			std::ranges::sort(_items, _comparer);
		}

		//! @brief Replaces the element at the specified index with a new value and re-sorts the vector
		//! @param index The zero-based index of the element to replace
		//! @param value The new value for the element
		void SetElementAt(size_t index, T&& value)
		{
			_items[index] = std::forward<T>(value);
			std::ranges::sort(_items, _comparer);
		}

		//! @brief Returns an iterator to the first element that is not less than the given item
		//! @param item The value to compare against
		auto LowerBound(const T& item) const
		{
			return std::ranges::lower_bound(_items, item, _comparer);
		}

		//! @brief Returns an iterator to the first element that is greater than the given item
		//! @param item The value to compare against
		auto UpperBound(const T& item) const
		{
			return std::ranges::upper_bound(_items, item, _comparer);
		}

		//! @brief Searches for the specified item and returns the zero-based index of the first occurrence
		//! @param item The item to locate
		//! @return The zero-based index if found; otherwise, -1
		int Find(const T& item) const
		{
			auto it = LowerBound(item);
			if(it != _items.end() && !_comparer(item, *it))
			{
				return static_cast<int>(it - _items.begin());
			}

			return -1;
		}

		//! @brief Removes the first occurrence of a specific item from the vector
		//! @param item The item to remove
		void Remove(const T& item)
		{
			auto it = LowerBound(item);
			if(it != _items.end() && !_comparer(item, *it))
			{
				_items.erase(it);
			}
		}

		//! @brief Removes the element at the specified index
		//! @param index The zero-based index of the element to remove
		void RemoveAt(size_t index)
		{
			_items.erase(_items.begin() + index);
		}

		//! @brief Returns a const reference to the element at the specified index using operator syntax
		//! @param index The zero-based index of the element to get
		//! @return A const reference to the element
		const T& operator[](size_t index) const
		{
			return _items[index];
		}

		//! @brief Assigns an initializer list to the vector and re-sorts it
		//! @param items The initializer list to assign
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
		[[no_unique_address]] TComparer _comparer;
	};
}