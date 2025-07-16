#pragma once

#include <list>
#include "EventArgs.h"
#include "Delegate.h"

namespace Sgl
{
	template<typename TSender, std::derived_from<EventArgs> TEventArgs>
	using EventHandler = Delegate<void(TSender&, const TEventArgs&)>;

	template<typename T>
	class Event;

	template<typename TSender, typename TEventArgs>
	class Event<EventHandler<TSender, TEventArgs>> final
	{
	public:
		using EventHandler = EventHandler<TSender, TEventArgs>;
	private:
		std::list<EventHandler> _eventHandlers;
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

		void TryInvoke(TSender& sender, const TEventArgs& e) const
		{
			if(HasTarget())
			{
				operator()(sender, e);
			}
		}

		bool HasTarget() const
		{
			return !_eventHandlers.empty();
		}

		void operator+=(EventHandler handler)
		{
			_eventHandlers.emplace_back(std::move(handler));
		}

		void operator-=(EventHandler handler)
		{
			_eventHandlers.remove(std::move(handler));
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