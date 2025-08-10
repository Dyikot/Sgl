#pragma once

#include "Event.h"
#include "ObservableProperty.h"

namespace Sgl
{
	class ObservableObject
	{
	private:
		std::unordered_map<size_t, Action<const void*>> _observers;
	public:
		void SetObserver(size_t propertyId, Action<const void*> observer)
		{
			_observers[propertyId] = std::move(observer);
		}

		void RemoveObserver(size_t propertyId)
		{
			_observers.erase(propertyId);
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