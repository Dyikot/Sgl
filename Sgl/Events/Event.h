#pragma once

#include <forward_list>
#include "EventHandler.h"

namespace Sgl
{
	template<typename THandler> requires IsEventHandler<THandler>
	class Event
	{
	protected:
		std::forward_list<THandler> _handlers;
	public:
		void Clear() noexcept { _handlers.clear(); }

		void operator+=(THandler&& handler)
		{
			_handlers.emplace_front(std::forward<THandler>(handler));
		}

		void operator+=(const THandler& handler)
		{
			_handlers.emplace_front(handler);
		}

		void operator-=(THandler&& handler)
		{
			_handlers.remove(std::forward<THandler>(handler));
		}

		void operator-=(const THandler& handler)
		{
			_handlers.remove(handler);
		}

		operator bool() const noexcept { return !_handlers.empty(); }

		void operator()(THandler::Object* sender, const THandler::EventArgs& e) const
		{
			for(const THandler& handler : _handlers)
			{
				handler(sender, e);
			}
		}
	};	
}