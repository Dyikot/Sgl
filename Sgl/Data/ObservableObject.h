#pragma once

#include <unordered_map>
#include "ObservableProperty.h"
#include "../Base/Delegate.h"

namespace Sgl
{
	class ObservableObject
	{
	private:
		std::unordered_map<size_t, Action<const void*>> _observers;
	public:
		ObservableObject() = default;
		ObservableObject(const ObservableObject&) = default;
		ObservableObject(ObservableObject&&) = default;
		~ObservableObject() = default;

		template<typename TObservable, typename TObserver, typename TMember>
		void SetObserver(ObservableProperty<TObservable, TMember>& observableProperty,
						 ObservableObject& observer,
						 ObservableProperty<TObserver, TMember>& observerProperty)
		{
			_observers[observableProperty.Id] = 
				[&observerProperty, obs = &static_cast<TObserver&>(observer)]
				(const void* value)
			{
				(obs->*observerProperty.Setter)(AsValue<TMember>(value));
			};
		}

		template<typename TObservable, typename TObservableMember,
				 typename TObserver, typename TObserverMember,
				 typename TConverter>
		void SetObserver(ObservableProperty<TObservable, TObservableMember>& observableProperty,
						 ObservableObject& observer,
						 ObservableProperty<TObserver, TObserverMember>& observerProperty,
						 TConverter converter)
		{
			_observers[observableProperty.Id] =
				[&observerProperty, obs = &static_cast<TObserver&>(observer), converter]
				(const void* value)
			{
				(obs->*observerProperty.Setter)(converter(AsValue<TObservableMember>(value)));
			};
		}

		template<typename TObservable, typename TMember>
		void RemoveObserver(ObservableProperty<TObservable, TMember>& property)
		{
			_observers.erase(property.Id);
		}

		void RemoveAllObservers()
		{
			_observers.clear();
		}
	protected:
		template<typename TOwner, typename TMember, typename TField>
		void SetProperty(ObservableProperty<TOwner, TMember>& property, TField& field,
						 ObservableProperty<TOwner, TMember>::Value value)
		{
			if(field != value)
			{
				field = value;
				
				if(auto it = _observers.find(property.Id); it != _observers.end())
				{
					it->second(&value);
				}
			}
		}

		template<typename TOwner, typename TMember>
		void OnPropertyChanged(ObservableProperty<TOwner, TMember>& property,
							   ObservableProperty<TOwner, TMember>::Value value)
		{
			auto currentValue = std::invoke(property.Getter, this);

			if(currentValue != value)
			{
				if(auto it = _observers.find(property.Id); it != _observers.end())
				{
					it->second(&value);
				}
			}
		}
	private:
		template<typename T>
		static const T& AsValue(const void* value)
		{
			return *static_cast<const std::decay_t<T>*>(value);
		}
	};
}