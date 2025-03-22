#pragma once

#include <list>
#include "../Events/Event.h"

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
		using Base = std::vector<T>;
	public:
		NotifiableCollection() = default;

		NotifiableCollection(const NotifiableCollection& collection):
			Base(collection)
		{}

		NotifiableCollection(NotifiableCollection&& collection) noexcept:
			Base(std::move(collection))
		{}

		NotifiableCollection(size_t count, const T& value):
			Base(count, value)
		{}

		NotifiableCollection(std::initializer_list<T> initList):
			Base(initList)
		{}

		template<typename TIterator>
		NotifiableCollection(TIterator first, TIterator last):
			Base(first, last)
		{}

		void push_back(T&& item)
		{
			Base::push_back(std::forward<T>(item));
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Add,
					.StartIndex = std::distance(Base::begin(), Base::end() - 1),
					.EndIndex = std::distance(Base::begin(), Base::end() - 1)
				});
		}

		void push_back(const T& item)
		{
			Base::push_back(item);
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Add,
					.StartIndex = std::distance(Base::begin(), Base::end() - 1),
					.EndIndex = std::distance(Base::begin(), Base::end() - 1)
				});
		}

		void insert(Base::const_iterator position, const T& item)
		{
			auto newPosition = Base::insert(position, item);
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Add,
					.StartIndex = std::distance(Base::begin(), newPosition),
					.EndIndex = std::distance(Base::begin(), newPosition)
				});
		}

		void insert(Base::const_iterator position, T&& item)
		{
			auto newPosition = Base::insert(position, std::forward<T>(item));
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Add,
					.StartIndex = std::distance(Base::begin(), newPosition),
					.EndIndex = std::distance(Base::begin(), newPosition)
				});
		}

		void insert(Base::const_iterator position, size_t size, const T& item)
		{
			auto newPosition = Base::insert(position, size, item);
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Add,
					.StartIndex = std::distance(Base::begin(), newPosition),
					.EndIndex = std::distance(Base::begin(), newPosition + size - 1)
				});
		}

		template<typename TIterator> requires std::_Is_iterator_v<TIterator>
		void insert(Base::const_iterator position, TIterator first, TIterator last)
		{
			auto newPosition = Base::insert(position, first, last);
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Add,
					.StartIndex = std::distance(Base::begin(), newPosition),
					.EndIndex = std::distance(Base::begin(), 
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
					.EndIndex = std::distance(Base::begin(), Base::end())
				});
			Base::clear();
		}

		template<typename... TArgs>
		void emplace(Base::const_iterator position, TArgs&&... args)
		{
			auto newPosition = Base::emplace(position, std::forward<TArgs>(args)...);
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Add,
					.StartIndex = std::distance(Base::begin(), newPosition),
					.EndIndex = std::distance(Base::begin(), newPosition)
				});
		}

		template<typename... TArgs>
		void emplace_back(TArgs&&... args)
		{
			Base::emplace_back(std::forward<TArgs>(args)...);
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Add,
					.StartIndex = std::distance(Base::begin(), Base::end()),
					.EndIndex = std::distance(Base::begin(), Base::end())
				});
		}

		void erase(Base::const_iterator position)
		{
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Remove,
					.StartIndex = std::distance(Base::cbegin(), position),
					.EndIndex = std::distance(Base::cbegin(), position)
				});
			Base::erase(position);
		}

		void erase(Base::const_iterator first, Base::const_iterator last)
		{
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Remove,
					.StartIndex = std::distance(Base::cbegin(), first),
					.EndIndex = std::distance(Base::cbegin(), last)
				});
			Base::erase(first, last);
		}

		void pop_back()
		{
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Remove,
					.StartIndex = std::distance(Base::begin(), Base::end()),
					.EndIndex = std::distance(Base::begin(), Base::end())
				});
			Base::pop_back();
		}

		void resize(size_t count)
		{
			if(count > Base::size())
			{
				auto oldSize = Base::size();
				Base::resize(count);
				OnChanged(
					NotifyEventArgs
					{
						.Action = NotifiableCollectionChangedAction::Add,
						.StartIndex = std::distance(Base::begin(), Base::begin() + oldSize),
						.EndIndex = std::distance(Base::begin(), Base::end() - 1)
					});
			}
			else if(count < Base::size())
			{
				OnChanged(
					NotifyEventArgs
					{
						.Action = NotifiableCollectionChangedAction::Remove,
						.StartIndex = std::distance(Base::begin(), Base::begin() + count),
						.EndIndex = std::distance(Base::begin(), Base::end() - 1)
					});
				Base::resize(count);
			}			
		}

		void move(Base::iterator from, Base::iterator where)
		{
			std::iter_swap(from, where);
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Move,
					.StartIndex = std::distance(Base::begin(), from),
					.EndIndex = std::distance(Base::begin(), where)
				});
		}

		void replace(Base::iterator position, const T& value)
		{
			*position = value;
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Replace,
					.StartIndex = std::distance(Base::begin(), position),
					.EndIndex = std::distance(Base::begin(), position)
				});
		}

		void replace(Base::iterator position, T&& value)
		{
			*position = std::forward<T>(value);
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Replace,
					.StartIndex = std::distance(Base::begin(), position),
					.EndIndex = std::distance(Base::begin(), position)
				});
		}

		void swap(NotifiableCollection& collection)
		{
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Remove,
					.StartIndex = std::distance(Base::begin(), Base::begin()),
					.EndIndex = std::distance(Base::begin(), Base::end())
				});
			Base::swap(collection);
			OnChanged(
				NotifyEventArgs
				{
					.Action = NotifiableCollectionChangedAction::Add,
					.StartIndex = std::distance(Base::begin(), Base::begin()),
					.EndIndex = std::distance(Base::begin(), Base::end())
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