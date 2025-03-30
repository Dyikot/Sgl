#pragma once

#include <concepts>
#include <functional>
#include "EventArgs.h"
#include "../Data/Nullable.h"

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
		Nullable<const std::type_info> _targetType;
	public:
		EventHandler() = default;

		EventHandler(CEventHandler<TSender, TEventArgs> auto&& handler):
			_handler(std::forward<decltype(handler)>(handler))
		{}

		EventHandler(CArgsEventHandler<TEventArgs> auto&& handler):
			_handler([handler](TSender& sender, const TEventArgs& e) { handler(e); }),
			_targetType(typeid(handler))
		{}

		EventHandler(std::invocable auto&& handler):
			_handler([handler](TSender& sender, const TEventArgs& e) { handler(); }),
			_targetType(typeid(handler))
		{}

		bool IsEmpty() const noexcept
		{ 
			return _handler;
		}

		const std::type_info& TargetType() const noexcept
		{
			return _targetType | _handler.target_type();
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