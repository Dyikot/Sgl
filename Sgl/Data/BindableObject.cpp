#include "BindableObject.h"

namespace Sgl
{
	BindableObject::BindableObject(const BindableObject& other):
		INotityPropertyChanged(other),
		_dataContext(other._dataContext),
		_observers(other._observers)
	{}

	BindableObject::BindableObject(BindableObject&& other) noexcept:
		INotityPropertyChanged(std::move(other)),
		_dataContext(std::move(other._dataContext)),
		_observers(std::move(other._observers))
	{}

	void BindableObject::SetDataContext(const Ref<INotityPropertyChanged>& value)
	{
		SetProperty(DataContextProperty, _dataContext, value);
	}

	void BindableObject::AddPropertyChangedEventHandler(SglPropertyBase& property, PropertyChangedEventHandler handler)
	{
		_observers.emplace_back(property, std::move(handler));
	}

	void BindableObject::RemovePropertyChangedEventHandler(SglPropertyBase& property, PropertyChangedEventHandler handler)
	{		
		Observer propertyObserver(property, std::move(handler));
		std::erase(_observers, propertyObserver);
	}	

	void BindableObject::NotifyPropertyChanged(SglPropertyBase& property)
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
