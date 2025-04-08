#pragma once

#include "EventArgs.h"
#include "../Data/Nullable.h"
#include "../Data/Delegates.h"

namespace Sgl
{
	template<typename TSender, CEventArgs TEventArgs>
	class EventHandler
	{
	private:
		Action<TSender&, const TEventArgs&> _handler;
		Nullable<const std::type_info> _targetType;
	public:
		EventHandler() = default;

		EventHandler(CAction<TSender, TEventArgs> auto&& handler):
			_handler(std::forward<decltype(handler)>(handler))
		{}

		EventHandler(CAction<TEventArgs> auto&& handler):
			_handler([handler](TSender& sender, const TEventArgs& e) { handler(e); }),
			_targetType(typeid(handler))
		{}

		EventHandler(CAction auto&& handler):
			_handler([handler](TSender& sender, const TEventArgs& e) { handler(); }),
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