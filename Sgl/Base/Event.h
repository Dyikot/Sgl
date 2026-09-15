#pragma once

#include <vector>
#include "Delegate.h"

namespace Sgl
{
	//! @brief Empty event args
	struct EventArgs {};

	//! @brief Represents a delegate for handling events with a sender and event arguments
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

		bool operator==(const MethodEventHandler& other) const noexcept
		{
			return _method == other._method;
		}

	private:
		Method _method;
		TSource* _source;
	};

	template<typename T>
	class Event;

	//! @brief Thread-unsafe event container for EventHandler delegates
	template<typename TSender, typename TEventArgs>
	class Event<EventHandler<TSender, TEventArgs>> final
	{
	public:
		using EventHandler = EventHandler<TSender, TEventArgs>;
		using EventHandlers = std::vector<EventHandler>;
		using Sender = TSender;
		using EventArgs = TEventArgs;

	public:
		Event() = default;
		Event(const Event&) = delete;
		Event(Event&&) noexcept = default;

		~Event()
		{
			Release();
		}

		//! @brief Removes all event handlers from the event
		void Clear() noexcept
		{
			Release();
		}

		//! @brief Checks whether the event has any registered handlers
		//! @return True if there are registered handlers; otherwise, false
		bool HasHandlers() const noexcept
		{
			return _eventHandlers != nullptr;
		}

		//! @brief Returns the number of event handlers
		//! @return Number of event handlers
		size_t Count() const noexcept
		{
			return _eventHandlers ? _eventHandlers->size() : 0;
		}

		//! @brief Adds an event handler to the event
		//! @param handler The event handler to add
		void operator+=(EventHandler handler)
		{
			if(!HasHandlers())
			{
				_eventHandlers = new EventHandlers();
			}

			_eventHandlers->push_back(std::move(handler));
		}

		//! @brief Removes an event handler from the event
		//! @param handler The event handler to remove
		void operator-=(const EventHandler& handler)
		{
			if(HasHandlers())
			{
				std::erase(*_eventHandlers, handler);
				if(_eventHandlers->empty())
				{
					Release();
				}
			}
		}

		//! @brief Invokes all registered event handlers with the specified sender and event arguments
		//! @param sender The sender object that is raising the event
		//! @param e The event arguments containing data about the event
		void Invoke(TSender& sender, TEventArgs e) const
		{
			if(HasHandlers())
			{
				for(auto& handler : *_eventHandlers)
				{
					handler(sender, e);
				}
			}
		}

		//! @brief Invokes event with default-constructed event arguments
		//! @param sender The sender object raising the event
		void Invoke(TSender& sender) const requires std::default_initializable<TEventArgs>
		{
			Invoke(sender, TEventArgs());
		}

		Event& operator=(const Event&) = delete;
		Event& operator=(Event&&) = delete;

	private:
		void Release()
		{
			delete _eventHandlers;
			_eventHandlers = nullptr;
		}

	private:
		EventHandlers* _eventHandlers = nullptr;
	};

	template<typename T>
	struct is_event: std::false_type {};

	template<typename T>
	struct is_event<Event<T>>: std::true_type {};

	template<typename T>
	concept CEvent = is_event<T>::value;
}