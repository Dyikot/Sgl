#pragma once
#include <vector>
#include <ranges>
#include <concepts>

namespace Sgl
{
	//! @brief Provides a base class for a generic collection that can be customized through inheritance
	//! @tparam T The type of elements in the collection. Must be equality comparable.
	template<std::equality_comparable T>
	class Collection
	{
	public:
		//! @brief Default constructor
		Collection() = default;

		//! @brief Initializes a new instance of the Collection class with the specified initializer list
		//! @param init The initial elements for the collection
		Collection(std::initializer_list<T> init):
			_items(init)
		{}

		//! @brief Initializes a new instance of the Collection class with the specified vector
		//! @param range The vector containing initial elements
		explicit Collection(std::vector<T> range):
			_items(std::move(range))
		{}

		Collection(const Collection&) = default;
		Collection(Collection&&) noexcept = default;
		virtual ~Collection() = default;

		//! @brief Returns an iterator to the beginning of the collection
		auto begin() const { return _items.begin(); }

		//! @brief Returns an iterator to the end of the collection
		auto end() const { return _items.end(); }

		//! @brief Returns a reverse iterator to the beginning of the collection
		auto rbegin() const { return _items.rbegin(); }

		//! @brief Returns a reverse iterator to the end of the collection
		auto rend() const { return _items.rend(); }

		//! @brief Gets the number of elements in the collection
		size_t Count() const noexcept
		{
			return _items.size();
		}

		//! @brief Determines whether the collection is empty
		bool IsEmpty() const noexcept
		{
			return _items.empty();
		}

		//! @brief Returns a const reference to the first element in the collection
		const T& Front() const
		{
			return _items.front();
		}

		//! @brief Returns a const reference to the last element in the collection
		const T& Back() const
		{
			return _items.back();
		}

		//! @brief Adds an item to the end of the collection
		//! @param item The item to add
		void Add(const T& item)
		{
			InsertItem(_items.size(), item);
		}

		//! @brief Removes all elements from the collection
		void Clear()
		{
			ClearItems();
		}

		//! @brief Determines whether an element is in the collection
		//! @param item The item to locate
		//! @return True if the item is found; otherwise, false
		bool Contains(const T& item) const
		{
			return std::ranges::find(_items, item) != _items.end();
		}

		//! @brief Searches for the specified item and returns the zero-based index of the first occurrence
		//! @param item The item to locate
		//! @return The zero-based index if found; otherwise, -1
		int IndexOf(const T& item) const
		{
			auto it = std::ranges::find(_items, item);
			return it != _items.end() ? static_cast<int>(it - _items.begin()) : -1;
		}

		//! @brief Inserts an item into the collection at the specified index
		//! @param index The zero-based index at which the item should be inserted
		//! @param item The item to insert
		void Insert(size_t index, const T& item)
		{
			InsertItem(index, item);
		}

		//! @brief Removes the first occurrence of a specific item from the collection
		//! @param item The item to remove
		//! @return True if the item was successfully removed; otherwise, false
		bool Remove(const T& item)
		{
			auto it = std::ranges::find(_items, item);
			if(it != _items.end())
			{
				RemoveItem(static_cast<size_t>(it - _items.begin()));
				return true;
			}
			return false;
		}

		//! @brief Removes the element at the specified index
		//! @param index The zero-based index of the element to remove
		void RemoveAt(size_t index)
		{
			RemoveItem(index);
		}

		//! @brief Replaces the element at the specified index with a new value
		//! @param index The zero-based index of the element to replace
		//! @param item The new value for the element
		void SetElementAt(size_t index, const T& item)
		{
			SetItem(index, item);
		}

		//! @brief Returns a const reference to the element at the specified index
		//! @param index The zero-based index of the element to get
		const T& GetElementAt(size_t index) const
		{
			return _items.at(index);
		}

		//! @brief Returns a const reference to the element at the specified index using operator syntax
		//! @param index The zero-based index of the element to get
		//! @return A const reference to the element
		const T& operator[](size_t index) const
		{
			return _items[index];
		}

		Collection& operator=(const Collection&) = default;
		Collection& operator=(Collection&&) noexcept = default;

	protected:
		virtual void ClearItems()
		{
			_items.clear();
		}

		virtual void InsertItem(size_t index, const T& item)
		{
			_items.insert(_items.begin() + index, item);
		}

		virtual void SetItem(size_t index, const T& item)
		{
			_items[index] = item;
		}

		virtual void RemoveItem(size_t index)
		{
			_items.erase(_items.begin() + index);
		}

	protected:
		std::vector<T> _items;
	};
}