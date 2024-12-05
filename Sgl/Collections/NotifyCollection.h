#pragma once

#include <list>
#include "../Events/Event.h"

namespace Sgl
{
	struct NotifyEventArgs: EventArgs
	{
		const void* Source;
	};

	template<typename T>
	class NotifyCollection: protected std::list<T>
	{
	public:
		using Base = std::list<T>;
		using NofityEventHandler = EventHandler<void, NotifyEventArgs>;
	public:
		Base::iterator begin() { return Base::begin(); }
		Base::const_iterator cbegin() { return Base::cbegin(); }
		Base::iterator end() { return Base::end(); }
		Base::const_iterator cend() { return Base::cend(); }

		void Add(T&& item)
		{
			Base::push_back(std::forward<T>(item));
			OnAdded(NotifyEventArgs{ .Source = &item });
		}

		void Add(const T& item)
		{
			Base::push_back(item);
			OnAdded(NotifyEventArgs{ .Source = &item });
		}

		void Insert(Base::const_iterator position, const T& item)
		{
			Base::insert(position, item);
			OnAdded(NotifyEventArgs{ .Source = &item });
		}

		void Insert(Base::const_iterator position, T&& item)
		{
			Base::insert(position, std::forward<T>(item));
			OnAdded(NotifyEventArgs{ .Source = &item });
		}

		void Clear()
		{
			for(T& item : *this)
			{
				OnRemoved(NotifyEventArgs{ .Source = &item });
			}

			Base::clear();
		}

		void Remove(const T& item)
		{
			Base::remove(item);
			OnRemoved(NotifyEventArgs{ .Source = &item });
		}

		void RemoveLast()
		{
			OnRemoved(NotifyEventArgs{ .Source = &Base::back() });
			Base::pop_back();
		}

		Event<NofityEventHandler> Added;
		Event<NofityEventHandler> Removed;
	protected:
		void OnAdded(const NotifyEventArgs& e)
		{
			if(Added)
			{
				Added(this, e);
			}
		}

		void OnRemoved(const NotifyEventArgs& e)
		{
			if(Removed)
			{
				Removed(this, e);
			}
		}
	};
}