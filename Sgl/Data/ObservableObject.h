#pragma once

#include <unordered_map>
#include "ObservableProperty.h"
#include "../Base/Observable/Event.h"

namespace Sgl
{
	class ObservableObject
	{
	private:
		std::unordered_map<size_t, Action<const void*>> _observers;
	public:
		template<typename TTarget, typename TValue>
		void SetObserver(ObservableProperty<TTarget, TValue>& property,
						 Action<const void*> observer)
		{
			_observers[property.Id] = std::move(observer);
		}

		template<typename TTarget, typename TValue>
		void RemoveObserver(ObservableProperty<TTarget, TValue>& property)
		{
			_observers.erase(property.Id);
		}
	protected:
		template<typename TOwner, typename TValue, typename TField>
		void SetProperty(ObservableProperty<TOwner, TValue>& property, TField& field,
						 ObservableProperty<TOwner, TValue>::Value value)
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
	};
}