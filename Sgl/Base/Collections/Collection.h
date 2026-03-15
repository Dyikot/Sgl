#pragma once

#include <vector>
#include <ranges>
#include <concepts>

namespace Sgl
{
	/// <summary>
	/// Provides a base class for a generic collection that can be customized through inheritance.
	/// </summary>
	/// <typeparam name="T">The type of elements in the collection. Must be equality comparable.</typeparam>
	template<std::equality_comparable T>
	class Collection
	{
	public:
		Collection() = default;

		/// <summary>
		/// Initializes a new instance of the Collection class with the specified initializer list.
		/// </summary>
		/// <param name="init"> - the initial elements for the collection.</param>
		Collection(std::initializer_list<T> init) :
			_items(init)
		{}

		/// <summary>
		/// Initializes a new instance of the Collection class with the specified vector.
		/// </summary>
		/// <param name="range"> - the vector containing initial elements.</param>
		explicit Collection(std::vector<T> range) :
			_items(std::move(range))
		{}

		Collection(const Collection&) = default;
		Collection(Collection&&) noexcept = default;
		virtual ~Collection() = default;

		/// <summary>
		/// Returns an iterator to the beginning of the collection.
		/// </summary>
		auto begin() const { return _items.begin(); }

		/// <summary>
		/// Returns an iterator to the end of the collection.
		/// </summary>
		auto end() const { return _items.end(); }

		/// <summary>
		/// Returns a reverse iterator to the beginning of the collection.
		/// </summary>
		auto rbegin() const { return _items.rbegin(); }

		/// <summary>
		/// Returns a reverse iterator to the end of the collection.
		/// </summary>
		auto rend() const { return _items.rend(); }

		/// <summary>
		/// Gets the number of elements in the collection.
		/// </summary>
		size_t Count() const noexcept
		{
			return _items.size();
		}

		/// <summary>
		/// Determines whether the collection is empty.
		/// </summary>
		bool IsEmpty() const noexcept
		{
			return _items.empty();
		}

		/// <summary>
		/// Returns a const reference to the first element in the collection.
		/// </summary>
		const T& Front() const
		{
			return _items.front();
		}

		/// <summary>
		/// Returns a const reference to the last element in the collection.
		/// </summary>
		const T& Back() const
		{
			return _items.back();
		}

		/// <summary>
		/// Adds an item to the end of the collection.
		/// </summary>
		/// <param name="item"> - the item to add.</param>
		void Add(const T& item)
		{
			InsertItem(_items.size(), item);
		}

		/// <summary>
		/// Removes all elements from the collection.
		/// </summary>
		void Clear()
		{
			ClearItems();
		}

		/// <summary>
		/// Determines whether an element is in the collection.
		/// </summary>
		/// <param name="item"> - the item to locate.</param>
		/// <returns>True if the item is found; otherwise, false.</returns>
		bool Contains(const T& item) const
		{
			return std::ranges::find(_items, item) != _items.end();
		}

		/// <summary>
		/// Searches for the specified item and returns the zero-based index of the first occurrence.
		/// </summary>
		/// <param name="item"> - the item to locate.</param>
		/// <returns>The zero-based index if found; otherwise, -1.</returns>
		int IndexOf(const T& item) const
		{
			auto it = std::ranges::find(_items, item);
			return it != _items.end() ? static_cast<int>(it - _items.begin()) : -1;
		}

		/// <summary>
		/// Inserts an item into the collection at the specified index.
		/// </summary>
		/// <param name="index"> - the zero-based index at which the item should be inserted.</param>
		/// <param name="item">The item to insert.</param>
		void Insert(size_t index, const T& item)
		{
			InsertItem(index, item);
		}

		/// <summary>
		/// Removes the first occurrence of a specific item from the collection.
		/// </summary>
		/// <param name="item"> - the item to remove.</param>
		/// <returns>True if the item was successfully removed; otherwise, false.</returns>
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

		/// <summary>
		/// Removes the element at the specified index.
		/// </summary>
		/// <param name="index"> - the zero-based index of the element to remove.</param>
		void RemoveAt(size_t index)
		{
			RemoveItem(index);
		}

		/// <summary>
		/// Replaces the element at the specified index with a new value.
		/// </summary>
		/// <param name="index"> - the zero-based index of the element to replace.</param>
		/// <param name="item"> - the new value for the element.</param>
		void SetElementAt(size_t index, const T& item)
		{
			SetItem(index, item);
		}

		/// <summary>
		/// Returns a const reference to the element at the specified index.
		/// </summary>
		/// <param name="index"> - the zero-based index of the element to get.</param>
		const T& GetElementAt(size_t index) const
		{
			return _items.at(index);
		}

		/// <summary>
		/// Returns a const reference to the element at the specified index using operator syntax.
		/// </summary>
		/// <param name="index"> - the zero-based index of the element to get.</param>
		/// <returns>A const reference to the element.</returns>
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