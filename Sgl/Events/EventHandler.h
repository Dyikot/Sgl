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
	public:
		EventHandler() = default;

		EventHandler(const EventHandler& other):
			_handler(other._handler)
		{}

		EventHandler(EventHandler&& other) noexcept:
			_handler(std::move(other._handler))
		{}

		EventHandler(Action<TSender&, const TEventArgs&> handler):
			_handler(std::move(handler))
		{}

		bool IsEmpty() const noexcept
		{ 
			return _handler.IsEmpty();
		}

		const std::type_info& TargetType() const noexcept
		{
			return _handler.TargetType();
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