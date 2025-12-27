#include "ObservableObject.h"

namespace Sgl
{
	void ObservableObject::AddPropertyChangedEventHandler(SglPropertyBase& property, PropertyChangedEventHandler handler)
	{
		_observers.emplace_back(property, std::move(handler));
	}

	void ObservableObject::RemovePropertyChangedEventHandler(SglPropertyBase& property, PropertyChangedEventHandler handler)
	{
		Observer propertyObserver(property, std::move(handler));
		std::erase(_observers, propertyObserver);
	}

	void ObservableObject::NotifyPropertyChanged(SglPropertyBase& property)
	{
		for(auto& observer : _observers)
		{
			if(observer.Property == property)
			{
				observer.Handler(*this, property);
				return;
			}
		}
	}
}

