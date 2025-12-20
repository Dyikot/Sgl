#pragma once

#include <vector>
#include "ObservableProperty.h"
#include "../Base/Delegate.h"

namespace Sgl
{
	class ObservableObject
	{
	private:
		using Observer = Action<const void*>;

		std::vector<Observer> _observers;
	public:
		ObservableObject(): _observers(ObservablePropertyBase::_nextId) {}
		ObservableObject(const ObservableObject&) = default;
		ObservableObject(ObservableObject&&) = default;
		virtual ~ObservableObject() = default;

		template<typename TObservable, typename TObserver, typename TValue>
		void SetObserver(ObservableProperty<TObservable, TValue>& observableProperty,
						 ObservableObject& observer,
						 ObservableProperty<TObserver, TValue>& observerProperty)
		{
			_observers[observableProperty.Id] = 
				[&observerProperty, obs = &static_cast<TObserver&>(observer)]
				(const void* value)
			{
				const auto& val = *static_cast<const std::decay_t<TValue>*>(value);
				observerProperty.Set(*obs, val);
			};
		}

		template<typename TObservable, typename TObservableValue,
				 typename TObserver, typename TObserverValue,
				 typename TConverter>
		void SetObserver(ObservableProperty<TObservable, TObservableValue>& observableProperty,
						 ObservableObject& observer,
						 ObservableProperty<TObserver, TObserverValue>& observerProperty,
						 TConverter converter)
		{
			_observers[observableProperty.Id] =
				[&observerProperty, obs = &static_cast<TObserver&>(observer), converter]
				(const void* value)
			{
				const auto& val = *static_cast<const std::decay_t<TObservableValue>*>(value);
				observerProperty.Set(*obs, converter(val));
			};
		}

		void RemoveObserver(ObservablePropertyBase& property)
		{
			_observers[property.Id] = nullptr;
		}

		void RemoveAllObservers()
		{
			_observers.clear();
		}
	protected:
		template<typename TOwner, typename TMember, typename TField>
		bool SetProperty(ObservableProperty<TOwner, TMember>& property, TField& field,
						 ObservableProperty<TOwner, TMember>::Value value)
		{
			if(field != value)
			{
				field = value;
				
				if(auto& observer = _observers[property.Id])
				{
					observer(&value);
				}

				return true;
			}

			return false;
		}
	};
}