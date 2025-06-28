#pragma once

#include <forward_list>
#include "EventHandler.h"

namespace Sgl
{
	template<typename T>
	class Event;

	template<typename TSender, std::derived_from<EventArgs> TEventArgs>
	class Event<EventHandler<TSender, TEventArgs>> final
	{
	public:
		using EventHandler = EventHandler<TSender, TEventArgs>;
	private:
		std::forward_list<EventHandler> _eventHandlers;
	public:
		Event() = default;

		Event(const Event& other):
			_eventHandlers(other._eventHandlers)
		{}

		Event(Event&& other) noexcept:
			_eventHandlers(std::move(other._eventHandlers))
		{}

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

		bool HasTarget() const
		{
			return !_eventHandlers.empty();
		}

		void operator+=(Action<TSender&, const TEventArgs&> handler)
		{
			_eventHandlers.emplace_front(std::move(handler));
		}

		void operator-=(Action<TSender&, const TEventArgs&> handler)
		{
			_eventHandlers.remove(std::move(handler));
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