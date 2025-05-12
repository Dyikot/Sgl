#pragma once

#include "EventArgs.h"
#include "../Data/Nullable.h"
#include "../Data/Delegate.h"

namespace Sgl
{
	template<typename TSender, std::derived_from<EventArgs> TEventArgs>
	class EventHandler
	{
	private:
		Action<TSender&, const TEventArgs&> _handler;
		Nullable<const std::type_info> _targetType;
	public:
		EventHandler() = default;

		EventHandler(CAction<TSender&, const TEventArgs&> auto&& handler):
			_handler(std::forward<decltype(handler)>(handler)),
			_targetType(typeid(_handler))
		{}

		EventHandler(CAction<const TEventArgs&> auto&& handler):
			_handler([h = std::forward<decltype(handler)>(handler)]
					 (TSender& sender, const TEventArgs& e) { h(e); }),
			_targetType(typeid(handler))
		{}

		EventHandler(CAction auto&& handler):
			_handler([h = std::forward<decltype(handler)>(handler)]
					 (TSender& sender, const TEventArgs& e) { h(); }),
			_targetType(typeid(handler))
		{}

		bool IsEmpty() const noexcept
		{ 
			return _handler.IsEmpty();
		}

		const std::type_info& TargetType() const noexcept
		{
			return _targetType | _handler.TargetType();
		}

		void Invoke(TSender& sender, const TEventArgs& e) const
		{
			_handler(sender, e);
		}

		void TryInvoke(TSender& sender, const TEventArgs& e) const noexcept
		{
			if(!IsEmpty())
			{
				_handler(sender, e);
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
	};
}