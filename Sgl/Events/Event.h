#pragma once

#include <forward_list>
#include <functional>
#include "EventArgs.h"

namespace Sgl
{
	template<typename TObject, typename TEventArgs> requires std::derived_from<TEventArgs, EventArgs>
	using EventHandler = std::function<void(TObject*, const TEventArgs&)>;

	template<typename T>
	class Event;

	template<typename TObject, typename TEventArgs> requires std::derived_from<TEventArgs, EventArgs>
	class Event<EventHandler<TObject, TEventArgs>> final
	{
	public:
		using EventHandler = EventHandler<TObject, TEventArgs>;
	protected:
		std::forward_list<EventHandler> _eventHandlers;
	public:
		void Clear() noexcept { _eventHandlers.clear(); }

		template<typename TCallable> 
			requires !std::is_same_v<std::decay_t<TCallable>, EventHandler> &&
					  std::constructible_from<EventHandler, TCallable>
		void operator+=(TCallable&& callable)
		{
			_eventHandlers.emplace_front(std::forward<TCallable>(callable));
		}

		void operator+=(EventHandler&& eventHandler)
		{
			_eventHandlers.push_front(std::move(eventHandler));
		}

		void operator+=(const EventHandler& eventHandler)
		{
			_eventHandlers.push_front(eventHandler);
		}

		template<typename TCallable> 
			requires !std::is_same_v<std::decay_t<TCallable>, EventHandler> &&
					  std::constructible_from<EventHandler, TCallable>
		void operator-=(TCallable&& callable)
		{
			_eventHandlers.remove(std::forward<TCallable>(callable));
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

		void operator()(TObject* sender, const TEventArgs& e) const
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