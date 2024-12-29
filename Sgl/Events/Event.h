#pragma once

#include <forward_list>
#include "EventHandler.h"

namespace Sgl
{
	template<typename TEventHandler, 
			 typename TObject = typename TEventHandler::Object,
			 typename TEventArgs = typename TEventHandler::EventArgs> requires
		std::is_base_of_v<EventHandler<typename TEventHandler::Object,
									   typename TEventHandler::EventArgs>, TEventHandler>
	class Event
	{
	public:
		using EventHanlder = TEventHandler;
	protected:
		std::forward_list<TEventHandler> _handlers;
	public:
		void Clear() noexcept { _handlers.clear(); }

		template<typename TCallable> 
			requires !std::is_same_v<std::decay_t<TCallable>, TEventHandler>
		void operator+=(TCallable&& callable)
		{
			_handlers.emplace_front(std::forward<TCallable>(callable));
		}

		void operator+=(TEventHandler&& handler)
		{
			_handlers.push_front(std::move(handler));
		}

		void operator+=(const TEventHandler& handler)
		{
			_handlers.push_front(handler);
		}

		template<typename TCallable> 
			requires !std::is_same_v<std::decay_t<TCallable>, TEventHandler>
		void operator-=(TCallable&& callable)
		{
			_handlers.remove(std::forward<TCallable>(callable));
		}

		void operator-=(TEventHandler&& handler)
		{
			_handlers.remove(std::move(handler));
		}

		void operator-=(const TEventHandler& handler)
		{
			_handlers.remove(handler);
		}

		operator bool() const noexcept { return !_handlers.empty(); }

		void operator()(TObject* sender, const TEventArgs& e) const
		{
			for(const TEventHandler& handler : _handlers)
			{
				handler(sender, e);
			}
		}
	};	
}