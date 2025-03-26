#pragma once

#include <concepts>
#include <functional>
#include "EventArgs.h"

namespace Sgl
{
	template<typename TInvocable, typename TSender, typename TEventArgs>
	concept CEventHandler = CEventArgs<TEventArgs> &&
		std::invocable<TInvocable, TSender&, const TEventArgs&>;

	template<typename TInvocable, typename TEventArgs>
	concept CArgsEventHandler = CEventArgs<TEventArgs> &&
		std::invocable<TInvocable, const TEventArgs&>;

	template<typename TSender, CEventArgs TEventArgs>
	class EventHandler
	{
	private:
		std::function<void(TSender&, const TEventArgs&)> _handler;
	public:
		EventHandler() = default;

		EventHandler(CEventHandler<TSender, TEventArgs> auto&& handler)
			: _handler(std::forward<decltype(handler)>(handler))
		{}

		bool IsEmpty() const noexcept
		{ 
			return _handler;
		}

		const std::type_info& TargetType() const noexcept
		{
			return _handler.target_type();
		}

		void Invoke(TSender& sender, const TEventArgs& e) const
		{
			_handler(sender, e);
		}

		void TryInvoke(TSender& sender, const TEventArgs& e) const noexcept
		{
			if(!IsEmpty())
			{
				Invoke(sender, e);
			}
		}

		void operator()(TSender& sender, const TEventArgs& e) const
		{ 
			_handler(sender, e);
		}

		friend bool operator==(const EventHandler& left, const EventHandler& right) noexcept
		{
			return left.TargetType() == right.TargetType();
		}

		friend bool operator!=(const EventHandler& left, const EventHandler& right) noexcept
		{
			return !operator==(left, right);
		}
	};
}