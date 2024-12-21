#pragma once

#include <forward_list>
#include <functional>

namespace Sgl
{
	struct EventArgs {};

	template<typename TObject, typename TEventArgs> requires std::derived_from<TEventArgs, EventArgs>
	using EventHandler = std::function<void(TObject*, const TEventArgs&)>;

	template<typename THandler>
	class Event
	{
	protected:
		std::forward_list<THandler> _handlers;
	public:
		void operator+=(auto&& handler) 
		{ 
			_handlers.emplace_front(std::forward<decltype(handler)>(handler));
		}

		void operator-=(auto&& handler) 
		{ 
			_handlers.remove(std::forward<decltype(handler)>(handler));
		}

		operator bool() const noexcept
		{ 
			return !_handlers.empty();
		}

		template<typename TObject, typename TEventArgs> requires std::derived_from<TEventArgs, EventArgs>
		void operator()(TObject* sender, const TEventArgs& e) const
		{
			for(const THandler& handler : _handlers)
			{
				handler(sender, e);
			}
		}

		void Clear() noexcept
		{ 
			_handlers.clear(); 
		}
	};	

	template <typename THandler>
	static bool operator==(const std::function<THandler>& lhs, 
						   const std::function<THandler>& rhs) noexcept
	{
		return lhs.target_type() == rhs.target_type();
	}
}