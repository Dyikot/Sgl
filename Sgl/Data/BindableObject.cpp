#include "BindableObject.h"

namespace Sgl
{
	BindableObject::BindableObject(const BindableObject& other):
		INotifyPropertyChanged(other),
		_dataContext(other._dataContext),
		_observers(other._observers)
	{}

	BindableObject::BindableObject(BindableObject&& other) noexcept:
		INotifyPropertyChanged(std::move(other)),
		_dataContext(std::move(other._dataContext)),
		_observers(std::move(other._observers))
	{}

	void BindableObject::SetDataContext(const Ref<INotifyPropertyChanged>& value)
	{
		SetDataContext(value, ValueSource::Local);
	}

	void BindableObject::SetDataContext(const Ref<INotifyPropertyChanged>& value, ValueSource source)
	{
		SetProperty(DataContextProperty, _dataContext, value, _dataContextSource, source);
	}

	void BindableObject::AddPropertyChangedEventHandler(AbstractPropertyBase& property, PropertyChangedEventHandler handler)
	{
		_observers.emplace_back(property, std::move(handler));
	}

	void BindableObject::RemovePropertyChangedEventHandler(AbstractPropertyBase& property, PropertyChangedEventHandler handler)
	{		
		Observer propertyObserver(property, std::move(handler));
		std::erase(_observers, propertyObserver);
	}	

	void BindableObject::NotifyPropertyChanged(AbstractPropertyBase& property)
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
