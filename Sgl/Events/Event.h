#pragma once

#include <list>
#include <functional>
#include "EventArgs.h"

namespace Sgl
{
	template<typename T>
	class Event
	{
	protected:
		std::list<T> _handlers;
	public:
		void operator+=(auto&& handler) 
		{ 
			_handlers.emplace_back(std::forward<decltype(handler)>(handler));
		}

		void operator-=(auto&& handler) 
		{ 
			_handlers.remove(std::forward<decltype(handler)>(handler));
		}

		operator bool() const 
		{ 
			return !_handlers.empty();
		}

		template<typename O, typename E> requires std::derived_from<E, EventArgs>
		void operator()(O* sender, const E& e) const
		{
			for(const T& handler : _handlers)
			{
				handler(sender, e);
			}
		}

		void Clear() 
		{ 
			_handlers.clear(); 
		}
	};	

	template <typename T>
	static bool operator==(const std::function<T>& lhs, const std::function<T>& rhs)
	{
		return lhs.target_type() == rhs.target_type();
	}
}