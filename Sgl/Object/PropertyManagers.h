#pragma once

#include "ObjectId.h"
#include "ObjectManager.h"
#include "../Any.h"

namespace Sgl
{
	using PropertyManager = ObjectManager<PropertyId>;

	class EventManager: public ObjectManager<EventId>
	{
	public:
		using InitializeEvent = void(&)(Any&, const Any&);
		using EventInitializersMap = std::unordered_map<EventId, InitializeEvent>;
		using Base = ObjectManager<EventId>;
	private:
		static inline EventInitializersMap _initEventMap;
	public:
		template<typename T>
		static const EventId Register(std::string&& name)
		{
			EventId id = Base::Register<T>(std::move(name));
			_initEventMap.emplace(id, SetEvent<T>);
			return id;
		}

		static const EventInitializersMap& GetEventInitializersMap() { return _initEventMap; }
	private:
		template<typename TEventHandler>
		static void SetEvent(Any& event, const Any& eventHandler)
		{
			event.As<Event<TEventHandler>>() += eventHandler.As<TEventHandler>();
		}
	};
}