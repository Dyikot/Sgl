#pragma once

#include <vector>
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
		int64_t StartIndex;
		int64_t EndIndex;
	};

	template<typename T>
	class NotifiableCollection: public std::vector<T>
	{
	public:
		using NofityEventHandler = EventHandler<NotifiableCollection<T>, NotifyEventArgs>;
		using base = std::vector<T>;
	public:
		NotifiableCollection() = default;

		NotifiableCollection(const NotifiableCollection& collection):
			base(collection)
		{}

		NotifiableCollection(NotifiableCollection&& collection) noexcept:
			base(std::move(collection))
		{}

		NotifiableCollection(size_t count, const T& value):
			base(count, value)
		{}

		NotifiableCollection(std::initializer_list<T> initList):
			base(initList)
		{}

		template<typename TIterator>
		NotifiableCollection(TIterator first, TIterator last):
			base(first, last)
		{}

		void push_back(T&& item)
		{
			base::push_back(std::forward<T>(item));
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Add,
					.StartIndex = std::distance(base::begin(), base::end() - 1),
					.EndIndex = std::distance(base::begin(), base::end() - 1)
				});
		}

		void push_back(const T& item)
		{
			base::push_back(item);
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Add,
					.StartIndex = std::distance(base::begin(), base::end() - 1),
					.EndIndex = std::distance(base::begin(), base::end() - 1)
				});
		}

		void insert(base::const_iterator position, const T& item)
		{
			auto newPosition = base::insert(position, item);
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Add,
					.StartIndex = std::distance(base::begin(), newPosition),
					.EndIndex = std::distance(base::begin(), newPosition)
				});
		}

		void insert(base::const_iterator position, T&& item)
		{
			auto newPosition = base::insert(position, std::forward<T>(item));
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Add,
					.StartIndex = std::distance(base::begin(), newPosition),
					.EndIndex = std::distance(base::begin(), newPosition)
				});
		}

		void insert(base::const_iterator position, size_t size, const T& item)
		{
			auto newPosition = base::insert(position, size, item);
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Add,
					.StartIndex = std::distance(base::begin(), newPosition),
					.EndIndex = std::distance(base::begin(), newPosition + size - 1)
				});
		}

		template<typename TIterator> requires std::_Is_iterator_v<TIterator>
		void insert(base::const_iterator position, TIterator first, TIterator last)
		{
			auto newPosition = base::insert(position, first, last);
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Add,
					.StartIndex = std::distance(base::begin(), newPosition),
					.EndIndex = std::distance(base::begin(), 
											  newPosition + std::distance(first, last) - 1)
				});
		}

		void clear() noexcept
		{
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Remove,
					.StartIndex = 0,
					.EndIndex = std::distance(base::begin(), base::end())
				});
			base::clear();
		}

		template<typename... TArgs>
		void emplace(base::const_iterator position, TArgs&&... args)
		{
			auto newPosition = base::emplace(position, std::forward<TArgs>(args)...);
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Add,
					.StartIndex = std::distance(base::begin(), newPosition),
					.EndIndex = std::distance(base::begin(), newPosition)
				});
		}

		template<typename... TArgs>
		void emplace_back(TArgs&&... args)
		{
			base::emplace_back(std::forward<TArgs>(args)...);
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Add,
					.StartIndex = std::distance(base::begin(), base::end()),
					.EndIndex = std::distance(base::begin(), base::end())
				});
		}

		void erase(base::const_iterator position)
		{
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Remove,
					.StartIndex = std::distance(base::cbegin(), position),
					.EndIndex = std::distance(base::cbegin(), position)
				});
			base::erase(position);
		}

		void erase(base::const_iterator first, base::const_iterator last)
		{
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Remove,
					.StartIndex = std::distance(base::cbegin(), first),
					.EndIndex = std::distance(base::cbegin(), last)
				});
			base::erase(first, last);
		}

		void pop_back()
		{
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Remove,
					.StartIndex = std::distance(base::begin(), base::end()),
					.EndIndex = std::distance(base::begin(), base::end())
				});
			base::pop_back();
		}

		void resize(size_t count)
		{
			if(count > base::size())
			{
				auto oldSize = base::size();
				base::resize(count);
				OnChanged(
					NotifyEventArgs
					{
						.Action = NotifiableCollectionChangedAction::Add,
						.StartIndex = std::distance(base::begin(), base::begin() + oldSize),
						.EndIndex = std::distance(base::begin(), base::end() - 1)
					});
			}
			else if(count < base::size())
			{
				OnChanged(
					NotifyEventArgs
					{
						.Action = NotifiableCollectionChangedAction::Remove,
						.StartIndex = std::distance(base::begin(), base::begin() + count),
						.EndIndex = std::distance(base::begin(), base::end() - 1)
					});
				base::resize(count);
			}			
		}

		void move(base::iterator from, base::iterator where)
		{
			std::iter_swap(from, where);
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Move,
					.StartIndex = std::distance(base::begin(), from),
					.EndIndex = std::distance(base::begin(), where)
				});
		}

		void replace(base::iterator position, const T& value)
		{
			*position = value;
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Replace,
					.StartIndex = std::distance(base::begin(), position),
					.EndIndex = std::distance(base::begin(), position)
				});
		}

		void replace(base::iterator position, T&& value)
		{
			*position = std::forward<T>(value);
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Replace,
					.StartIndex = std::distance(base::begin(), position),
					.EndIndex = std::distance(base::begin(), position)
				});
		}

		void swap(NotifiableCollection& collection)
		{
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Remove,
					.StartIndex = std::distance(base::begin(), base::begin()),
					.EndIndex = std::distance(base::begin(), base::end())
				});
			base::swap(collection);
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Add,
					.StartIndex = std::distance(base::begin(), base::begin()),
					.EndIndex = std::distance(base::begin(), base::end())
				});
		}

		Event<NofityEventHandler> Changed;
	protected:
		void OnChanged(const NotifyEventArgs& e)
		{
			Changed.TryInvoke(*this, e);
		}
	};
}