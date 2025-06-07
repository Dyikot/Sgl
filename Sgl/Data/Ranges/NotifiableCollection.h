#pragma once

#include <vector>
#include <ranges>
#include "../../Events/EventArgs.h"
#include "../../Events/Event.h"

namespace Sgl
{
	enum class NotifiableCollectionChangedAction
	{
		Add, Remove, Move, Replace
	};

	struct NotifyEventArgs: EventArgs
	{
		NotifiableCollectionChangedAction Action;
		size_t StartIndex;
		size_t EndIndex;
	};
	
	template<typename T>
	class NotifiableCollection
	{
	public:
		using NofityEventHandler = EventHandler<NotifiableCollection<T>, NotifyEventArgs>;

		Event<NofityEventHandler> Changed;
	private:
		std::vector<T> _items;
	public:
		NotifiableCollection() = default;

		NotifiableCollection(const NotifiableCollection& other):
			_items(other._items)
		{}

		NotifiableCollection(NotifiableCollection&& other) noexcept:
			_items(std::move(other._items))
		{}

		NotifiableCollection(size_t count):
			_items(count)
		{}

		NotifiableCollection(size_t count, const T& value):
			_items(count, value)
		{}

		NotifiableCollection(std::initializer_list<T> init):
			_items(init)
		{}

		template<std::ranges::range TRange>
			requires std::same_as<std::ranges::range_value_t<TRange>, T>
		NotifiableCollection(TRange&& range):
			_items(std::ranges::begin(range), std::ranges::end(range))
		{}

		auto begin() const { return _items.begin(); }
		auto end() const { return _items.end(); }

		void Add(const T& item)
		{
			_items.push_back(item);
			auto index = _items.size() - 1;

			OnChanged(NotifyEventArgs
			{
				.Action = NotifiableCollectionChangedAction::Add,
				.StartIndex = index,
				.EndIndex = index
			});
		}

		void Add(T&& item)
		{
			_items.push_back(std::forward<T>(item));
			auto index = _items.size() - 1;

			OnChanged(NotifyEventArgs
			{
				.Action = NotifiableCollectionChangedAction::Add,
				.StartIndex = index,
				.EndIndex = index
			});
		}

		size_t Count() const noexcept
		{
			return _items.size();
		}

		size_t Capacity() const noexcept
		{
			return _items.capacity();
		}

		void Insert(size_t index, const T& item)
		{
			_items.insert(_items.begin() + index, item);

			OnChanged(NotifyEventArgs
			{
				.Action = NotifiableCollectionChangedAction::Add,
				.StartIndex = index,
				.EndIndex = index
			});
		}

		void Insert(size_t index, T&& item)
		{
			_items.insert(_items.begin() + index, std::forward<T>(item));

			OnChanged(NotifyEventArgs
			{
				.Action = NotifiableCollectionChangedAction::Add,
				.StartIndex = index,
				.EndIndex = index
			});
		}

		template<std::ranges::range TRange> 
			requires std::same_as<std::ranges::range_value_t<TRange>, T>
		void InsertRange(size_t index, TRange&& range)
		{
			auto begin = std::ranges::begin(range);
			auto end = std::ranges::end(range);

			if(begin == end)
			{
				return;
			}

			_items.insert(_items.begin() + index, begin, end);

			OnChanged(NotifyEventArgs
			{
				.Action = NotifiableCollectionChangedAction::Add,
				.StartIndex = index,
				.EndIndex = index + std::ranges::distance(range) - 1
			});
		}

		void Clear() noexcept
		{
			OnChanged(NotifyEventArgs
			{
				.Action = NotifiableCollectionChangedAction::Remove,
				.StartIndex = 0,
				.EndIndex = _items.size() - 1
			});

			_items.clear();
		}

		template<typename... TArgs>
		void Emplace(size_t index, TArgs&&... args)
		{
			_items.emplace(_items.begin() + index, std::forward<TArgs>(args)...);

			OnChanged(NotifyEventArgs
			{
				.Action = NotifiableCollectionChangedAction::Add,
				.StartIndex = index,
				.EndIndex = index
			});
		}

		template<typename... TArgs>
		void EmplaceBack(TArgs&&... args)
		{
			_items.emplace_back(std::forward<TArgs>(args)...);
			auto index = _items.size() - 1;

			OnChanged(NotifyEventArgs
			{
				.Action = NotifiableCollectionChangedAction::Add,
				.StartIndex = index,
				.EndIndex = index
			});
		}

		void RemoveAt(size_t index)
		{
			OnChanged(NotifyEventArgs
			{
				.Action = NotifiableCollectionChangedAction::Remove,
				.StartIndex = index,
				.EndIndex = index
			});

			_items.erase(_items.begin() + index);
		}

		void RemoveLast()
		{
			auto index = _items.size() - 1;

			OnChanged(NotifyEventArgs
			{
				.Action = NotifiableCollectionChangedAction::Remove,
				.StartIndex = index,
				.EndIndex = index
			});

			_items.pop_back();
		}

		void RemoveRange(size_t index, size_t count)
		{
			OnChanged(NotifyEventArgs
			{
				.Action = NotifiableCollectionChangedAction::Remove,
				.StartIndex = index,
				.EndIndex = index + count - 1
			});

			auto begin = _items.begin() + index;
			_items.erase(begin, begin + count);
		}

		void Resize(size_t count)
		{
			auto size = _items.size();

			if(count > size)
			{
				_items.resize(count);

				OnChanged(NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Add,
					.StartIndex = size,
					.EndIndex = count - 1
				});
			}
			else if(count < size)
			{
				OnChanged(NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Remove,
					.StartIndex = count,
					.EndIndex = size - 1
				});

				_items.resize(count);
			}			
		}

		void Move(size_t oldIndex, size_t newIndex)
		{
			auto begin = _items.begin();
			std::iter_swap(begin + oldIndex, begin + newIndex);

			OnChanged(NotifyEventArgs
			{
				.Action = NotifiableCollectionChangedAction::Move,
				.StartIndex = oldIndex,
				.EndIndex = newIndex
			});
		}

		void SetItem(size_t index, const T& item)
		{
			_items[index] = item;

			OnChanged(NotifyEventArgs
			{
				.Action = NotifiableCollectionChangedAction::Replace,
				.StartIndex = index,
				.EndIndex = index
			});
		}

		void SetItem(size_t index, T&& item)
		{
			_items[index] = std::forward<T>(item);

			OnChanged(NotifyEventArgs
			{
				.Action = NotifiableCollectionChangedAction::Replace,
				.StartIndex = index,
				.EndIndex = index
			});
		}

		const T& ElementAt(size_t index)
		{
			return _items.at(index);
		}
	protected:
		void OnChanged(const NotifyEventArgs& e)
		{
			Changed.TryRaise(*this, e);
		}
	};
}