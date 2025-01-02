#pragma once

#include "PropertyManagers.h"
#include "../Collections/AnyMap.h"

namespace Sgl
{
	class Object
	{
	private:
		static inline const EventManager::EventInitializersMap& _eventInitializersMap =
			EventManager::GetEventInitializersMap();

		AnyMap<EventId> _events;
		AnyMap<PropertyId> _properties;
	protected:
		/* Events */

		template<typename TEventHandler>
		void AddEvent(EventId id)
		{
			_events.Add<Event<TEventHandler>>(id); 
		}

		template<typename TEventHandler>
		Event<TEventHandler>& GetEventValue(EventId id)
		{
			return _events[id].As<Event<TEventHandler>>();
		}

		template<typename TEventHandler>
		const Event<TEventHandler>& GetEventValue(EventId id) const
		{
			return _events.at(id).As<Event<TEventHandler>>();
		}

		Any& GetEvent(EventId id)
		{
			return _events[id];
		}

		const Any& GetEvent(EventId id) const 
		{ 
			return _events.at(id);
		}

		void SetEvent(EventId id, const Any& eventHandler)
		{
			_eventInitializersMap.at(id)(_events[id], eventHandler);
		}

		/* Properties */

		template<typename TValue, typename... TArgs>
		void AddProperty(PropertyId id, TArgs&&... args)
		{
			_properties.Add<TValue>(id, std::forward<TArgs>(args)...);
		}

		template<typename TValue>
		const TValue& GetPropertyValue(PropertyId id) const
		{ 
			return _properties.at(id).As<TValue>();
		}

		template<typename TValue>
		TValue& GetPropertyValue(PropertyId id) 
		{ 
			return _properties[id].As<TValue>();
		}

		Any& GetProperty(PropertyId id)
		{ 
			return _properties[id];
		}

		const Any& GetProperty(PropertyId id) const
		{ 
			return _properties.at(id);
		}

		template<typename TValue>
		void SetProperty(PropertyId id, const TValue& value)
		{ 
			_properties[id].As<TValue>() = value;
		}

		virtual void OnPropertyChanged(PropertyId id) {};
	};
}
