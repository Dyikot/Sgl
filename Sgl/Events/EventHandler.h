#pragma once

#include <functional>

namespace Sgl
{
	struct EventArgs {};

	template<typename TObject, typename TEventArgs> 
		requires std::derived_from<TEventArgs, EventArgs>
	class EventHandler: public std::function<void(TObject*, const TEventArgs&)>
	{
	public:
		using Base = std::function<void(TObject*, const TEventArgs&)>;
		using Object = TObject;
		using EventArgs = TEventArgs;
	public:
		EventHandler() = default;

		template<typename TCallable> 
			requires !std::is_same_v<std::decay_t<TCallable>, EventHandler>
		EventHandler(TCallable&& callable):
			Base(std::forward<TCallable>(callable))
		{}

		EventHandler(const EventHandler& handler):
			Base(static_cast<const Base&>(handler))
		{}

		EventHandler(EventHandler&& handler) noexcept:
			Base(static_cast<Base&&>(handler))
		{}

		template<typename TCallable> 
			requires !std::is_same_v<std::decay_t<TCallable>, EventHandler>
		EventHandler& operator=(TCallable&& callable)
		{
			Base::operator=(std::forward<TCallable>(callable));
			return *this;
		}

		EventHandler& operator=(const EventHandler& handler)
		{
			Base::operator=(static_cast<const Base&>(handler));
			return *this;
		}

		EventHandler& operator=(EventHandler&& handler) noexcept
		{
			Base::operator=(static_cast<Base&&>(handler));
			return *this;
		}

		bool operator==(const EventHandler& eventHandler) const noexcept
		{
			return this->target_type() == eventHandler.target_type();
		}

		bool operator!=(const EventHandler& eventHandler) const noexcept
		{
			return !operator==(eventHandler);
		}
	};
}