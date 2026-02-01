#pragma once

#include <vector>
#include "Delegate.h"

namespace Sgl
{
	/// <summary>
	/// Empty event args
	/// </summary>
	struct EventArgs {};

	/// <summary>
	/// Represents a delegate for handling events with a sender and event arguments.
	/// </summary>
	template<typename TSender, typename TEventArgs = EventArgs>
	using EventHandler = Delegate<void(TSender&, TEventArgs)>;

	template<typename TSource, typename TSender, typename TEventArgs>
	class MethodEventHandler
	{
	private:
		using Method = void(TSource::*)(TSender&, TEventArgs);
	public:
		MethodEventHandler(Method method, TSource* source):
			_method(method),
			_source(source)
		{}

		void operator()(TSender& sender, TEventArgs e) const
		{
			(_source->*_method)(sender, e);
		}

		friend bool operator==(const MethodEventHandler& left, const MethodEventHandler& right)
		{
			return left._method == right._method;
		}
	private:
		Method _method;
		TSource* _source;
	};

	template<typename T>
	class Event;

	/// <summary>
	/// Thread-unsafe event container for EventHandler delegates.
	/// </summary>
	template<typename TSender, typename TEventArgs>
	class Event<EventHandler<TSender, TEventArgs>> final
	{
	public:
		using EventHandler = EventHandler<TSender, TEventArgs>;
		using Sender = TSender;
		using EventArgs = TEventArgs;
	public:
		/// <summary>
		/// Default constructor. Creates an empty event with no handlers.
		/// </summary>
		Event() = default;
		Event(const Event&) = delete;
		Event(Event&&) = delete;

		/// <summary>
		/// Removes all event handlers from the event.
		/// </summary>
		void Clear() noexcept
		{ 
			_eventHandlers.clear();
		}

		/// <summary>
		/// Gets the number of registered handlers
		/// </summary>
		size_t Count() const noexcept
		{
			return _eventHandlers.size();
		}

		/// <summary>
		/// Checks whether the event has any registered handlers.
		/// </summary>
		/// <returns>True if there are registered handlers; otherwise, false.</returns>
		bool HasHandlers() const noexcept
		{
			return !_eventHandlers.empty();
		}

		/// <summary>
		/// Adds an event handler to the event.
		/// </summary>
		/// <param name="handler"> - The event handler to add.</param>
		void operator+=(EventHandler handler)
		{
			_eventHandlers.emplace_back(std::move(handler));
		}

		// <summary>
		/// Removes an event handler from the event.
		/// </summary>
		/// <param name="handler"> - The event handler to remove.</param>
		void operator-=(const EventHandler& handler)
		{
			std::erase(_eventHandlers, handler);
		}
	private:
		/// <summary>
		/// Invokes all registered event handlers with the specified sender and event arguments.
		/// </summary>
		/// <param name="sender"> - The sender object that is raising the event.</param>
		/// <param name="e"> - The event arguments containing data about the event.</param>
		void operator()(TSender& sender, TEventArgs e) const
		{
			for(const EventHandler& eventHandler : _eventHandlers)
			{
				eventHandler(sender, e);
			}
		}

		/// <summary>
		/// Invokes event with default-constructed event arguments.
		/// </summary>
		/// <param name="sender">The sender object raising the event</param>
		void operator()(TSender& sender) const requires std::default_initializable<TEventArgs>
		{
			operator()(sender, TEventArgs());
		}

		Event& operator=(const Event&) = delete;
		Event& operator=(Event&&) = delete;

		friend TSender;
	private:
		std::vector<EventHandler> _eventHandlers;
	};

	template<typename T>
	struct is_event : std::false_type {};

	template<typename T>
	struct is_event<Event<T>>: std::true_type {};

	template<typename T>
	concept CEvent = is_event<T>::value;
}