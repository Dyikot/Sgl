#include "ObservableObject.h"

namespace Sgl
{
	void ObservableObject::AddPropertyChangedEventHandler(PropertyBase& property, PropertyChangedEventHandler handler)
	{
		_propertiesObservers.emplace_back(property, std::move(handler));
	}

	void ObservableObject::RemovePropertyChangedEventHandler(PropertyBase& property, PropertyChangedEventHandler handler)
	{
		Observer propertyObserver(property, std::move(handler));
		std::erase(_propertiesObservers, propertyObserver);
	}

	void ObservableObject::NotifyPropertyChanged(PropertyBase& property)
	{
		for(auto& observer : _propertiesObservers)
		{
			if(observer.Property == property)
			{
				observer.Handler(*this, property);
				return;
			}
		}
	}
}

