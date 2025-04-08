#pragma once

#include <forward_list>
#include "EventHandler.h"

namespace Sgl
{
	template<typename T>
	class Event;

	template<typename TSender, CEventArgs TEventArgs>
	class Event<EventHandler<TSender, TEventArgs>> final
	{
	public:
		using EventHandler = EventHandler<TSender, TEventArgs>;
	protected:
		std::forward_list<EventHandler> _eventHandlers;
	public:
		void Clear() noexcept
		{ 
			_eventHandlers.clear();
		}

		void TryRaise(TSender& sender, const TEventArgs& e) const
		{
			if(operator bool())
			{
				operator()(sender, e);
			}
		}

		void Raise(TSender& sender, const TEventArgs& e) const
		{
			operator()(sender, e);
		}

		bool IsEmpty() const
		{
			return _eventHandlers.empty();
		}

		void operator+=(CAction<TSender, TEventArgs> auto&& handler)
		{
			_eventHandlers.emplace_front(std::forward<decltype(handler)>(handler));
		}

		void operator+=(CAction<TEventArgs> auto&& handler)
		{
			_eventHandlers.emplace_front(std::forward<decltype(handler)>(handler));
		}

		void operator+=(CAction auto&& handler)
		{
			_eventHandlers.emplace_front(std::forward<decltype(handler)>(handler));
		}

		void operator-=(CAction<TSender, TEventArgs> auto&& handler)
		{
			_eventHandlers.remove(std::forward<decltype(handler)>(handler));
		}

		void operator-=(CAction<TEventArgs> auto&& handler)
		{
			_eventHandlers.remove(std::forward<decltype(handler)>(handler));
		}

		void operator-=(CAction auto&& handler)
		{
			_eventHandlers.remove(std::forward<decltype(handler)>(handler));
		}

		operator bool() const noexcept
		{ 
			return !_eventHandlers.empty();
		}

		void operator()(TSender& sender, const TEventArgs& e) const
		{
			for(const EventHandler& eventHandler : _eventHandlers)
			{
				eventHandler(sender, e);
			}
		}
	};
}