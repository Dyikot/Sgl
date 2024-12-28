#pragma once

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
		EventHandler() = default;

		template<typename THandler> requires std::constructible_from<Base, THandler>
		EventHandler(THandler&& handler):
			Base(std::forward<THandler>(handler))
		{}

		EventHandler(const EventHandler& handler):
			Base(handler)
		{}

		EventHandler(EventHandler&& handler) noexcept:
			Base(std::move(handler))
		{}

		template<typename THandler> requires std::constructible_from<Base, THandler>
		EventHandler& operator=(THandler&& handler)
		{
			Base::operator=(std::forward<THandler>(handler));
			return *this;
		}

		EventHandler& operator=(const EventHandler& handler)
		{
			Base::operator=(handler);
			return *this;
		}

		EventHandler& operator=(EventHandler&& handler) noexcept
		{
			Base::operator=(std::move(handler));
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

	template<typename THandler>
	concept	IsEventHandler = std::same_as<THandler, EventHandler<typename THandler::Object,
		typename THandler::EventArgs>>;
}