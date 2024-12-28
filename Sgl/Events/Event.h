#pragma once

#include <forward_list>
#include <functional>

namespace Sgl
{
	struct EventArgs {};

	template<typename TObject, typename TEventArgs> requires std::derived_from<TEventArgs, EventArgs>
	class EventHandler: public std::function<void(TObject*, const TEventArgs&)>
	{
	public:
		using Base = std::function<void(TObject*, const TEventArgs&)>;
		using Object = TObject;
		using EventArgs = TEventArgs;
	public:
		EventHandler(auto&& handler):
			Base(std::forward<decltype(handler)>(handler))
		{}

		EventHandler& operator=(auto&& handler)
		{
			Base::operator=(std::forward<decltype(handler)>(handler));
			return *this;
		}

		bool operator==(const EventHandler& eventHandler) const noexcept
		{
			return this->target_type() == eventHandler.target_type();
		}
	};

	template<typename THandler> 
	concept	IsEventHandler = std::same_as<THandler, EventHandler<typename THandler::Object,
																 typename THandler::EventArgs>>;

	template<typename THandler> requires IsEventHandler<THandler>
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

		void operator()(THandler::Object* sender, const THandler::EventArgs& e) const
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

	/*template <typename THandler>
	static bool operator==(const std::function<THandler>& lhs, 
						   const std::function<THandler>& rhs) noexcept
	{
		return lhs.target_type() == rhs.target_type();
	}*/
}