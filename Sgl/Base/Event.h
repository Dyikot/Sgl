#pragma once

#include <vector>
#include "EventArgs.h"
#include "Delegate.h"

namespace Sgl
{
	template<typename TSender, std::derived_from<EventArgs> TEventArgs = EventArgs>
	using EventHandler = Delegate<void(TSender&, const TEventArgs&)>;

	template<typename T>
	class Event;

	template<typename TSender, typename TEventArgs>
	class Event<EventHandler<TSender, TEventArgs>> final
	{
	public:
		using EventHandler = EventHandler<TSender, TEventArgs>;
	private:
		std::vector<EventHandler> _eventHandlers;
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

		void operator-=(const EventHandler& handler)
		{
			std::erase(_eventHandlers, handler);
		}

		void operator()(TSender& sender, const TEventArgs& e) const
		{
			for(const EventHandler& eventHandler : _eventHandlers)
			{
				eventHandler(sender, e);
			}
		}

		Event& operator=(const Event& other)
		{
			if(this != &other)
			{
				_eventHandlers = other._eventHandlers;
			}

			return *this;
		}

		Event& operator=(Event&& other) noexcept
		{
			if(this != &other)
			{
				_eventHandlers = std::move(other._eventHandlers);
			}

			return *this;
		}
	};
}