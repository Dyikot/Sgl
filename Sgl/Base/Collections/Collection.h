#pragma once

#include <vector>
#include <ranges>
#include <concepts>

namespace Sgl
{
	template<std::equality_comparable T>
	class Collection
	{
	private:
		std::vector<T> _items;
	public:
		Collection() = default;

		Collection(std::initializer_list<T> init) :
			_items(init)
		{}

		explicit Collection(std::vector<T> range) :
			_items(std::move(range))
		{}

		Collection(const Collection&) = default;
		Collection(Collection&&) = default;
		virtual ~Collection() = default;

		auto begin() { return _items.begin(); }
		auto begin() const { return _items.begin(); }
		auto end() { return _items.end(); }
		auto end() const { return _items.end(); }

		size_t Count() const noexcept
		{
			return _items.size();
		}

		bool IsEmpty() const noexcept
		{
			return _items.size() == 0;
		}

		void Add(const T& item)
		{
			InsertItem(_items.size(), item);
		}

		void Clear()
		{
			ClearItems();
		}

		bool Contains(const T& item) const
		{
			return std::ranges::find(_items, item) != _items.end();
		}

		int IndexOf(const T& item) const
		{
			auto it = std::ranges::find(_items, item);
			return it != _items.end() ? static_cast<int>(it - _items.begin()) : -1;
		}

		void Insert(size_t index, const T& item)
		{
			InsertItem(index, item);
		}

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

		void RemoveAt(size_t index)
		{
			RemoveItem(index);
		}

		void SetElementAt(size_t index, const T& item)
		{
			SetItem(index, item);
		}

		const T& ElementAt(size_t index) const
		{
			return _items[index];
		}

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

		std::vector<T>& Items()
		{
			return _items;
		}

		const std::vector<T>& Items() const
		{
			return _items;
		}
	};
}