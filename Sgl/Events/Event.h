#pragma once

#include <forward_list>
#include <functional>
#include "EventArgs.h"

namespace Sgl
{
	template<typename TEventArgs>
	concept CEventArgs = std::derived_from<TEventArgs, EventArgs>;

	template<typename TInvocable, typename TObject, typename TEventArgs>
	concept CEventHandler = CEventArgs<TEventArgs> &&
							std::invocable<TInvocable, TObject&, const TEventArgs&>;

	template<typename TInvocable, typename TEventArgs>
	concept CArgsEventHandler = CEventArgs<TEventArgs> &&
								std::invocable<TInvocable, const TEventArgs&>;

	template<typename TObject, CEventArgs TEventArgs = EventArgs>
	using EventHandler = std::function<void(TObject&, const TEventArgs&)>;

	template<typename T>
	class Event;

	template<typename TObject, CEventArgs TEventArgs>
	class Event<EventHandler<TObject, TEventArgs>> final
	{
	public:
		using EventHandler = EventHandler<TObject, TEventArgs>;
	protected:
		std::forward_list<EventHandler> _eventHandlers;
	public:
		void Clear() noexcept { _eventHandlers.clear(); }

		void TryInvoke(TObject& sender, const TEventArgs& e) const noexcept
		{
			if(operator bool())
			{
				operator()(sender, e);
			}
		}

		void operator+=(CEventHandler<TObject, TEventArgs> auto&& handler)
		{
			_eventHandlers.emplace_front(std::forward<decltype(handler)>(handler));
		}

		void operator+=(CArgsEventHandler<TEventArgs> auto&& handler)
		{
			_eventHandlers.emplace_front([handler](TObject& sender, const TEventArgs& e) { handler(e); });
		}

		void operator+=(std::invocable auto&& handler)
		{
			_eventHandlers.emplace_front([handler](TObject& sender, const TEventArgs& e) { handler(); });
		}

		void operator+=(EventHandler&& eventHandler)
		{
			_eventHandlers.push_front(std::move(eventHandler));
		}

		void operator+=(const EventHandler& eventHandler)
		{
			_eventHandlers.push_front(eventHandler);
		}

		void operator-=(CEventHandler auto&& hanlder)
		{
			_eventHandlers.remove(std::forward<decltype(hanlder)>(hanlder));
		}

		void operator-=(EventHandler&& eventHandler)
		{
			_eventHandlers.remove(std::move(eventHandler));
		}

		void operator-=(const EventHandler& eventHandler)
		{
			_eventHandlers.remove(eventHandler);
		}

		operator bool() const noexcept { return !_eventHandlers.empty(); }

		void operator()(TObject& sender, const TEventArgs& e) const
		{
			for(const EventHandler& eventHandler : _eventHandlers)
			{
				eventHandler(sender, e);
			}
		}
	};

	template<typename T>
	inline bool operator==(const std::function<T>& left, const std::function<T>& right) noexcept
	{
		return left.target_type() == right.target_type();
	}

	template<typename T>
	inline bool operator!=(const std::function<T>& left, const std::function<T>& right) noexcept
	{
		return !operator==(left, right);
	}
}