#include "BindableObject.h"
#include <algorithm>

namespace Sgl
{
	BindableObject::BindingContext::BindingContext(BindingBase* binding, BindableObject* bindableObject):
		_binding(binding), 
		_bindgableObject(bindableObject)
	{}

	void BindableObject::BindingContext::Clear()
	{
		auto& bindings = _bindgableObject->_bindings;
		auto proj = &std::unique_ptr<BindingBase>::get;

		if(auto it = std::ranges::find(bindings, _binding, proj); it != bindings.end())
		{
			if(_binding->IsApplied())
			{
				_binding->Clear(*_bindgableObject);
			}

			bindings.erase(it);
		}
	}

	BindableObject::BindableObject(const BindableObject& other):
		INotifyPropertyChanged(other),
		_dataContext(other._dataContext)
	{}

	BindableObject::BindableObject(BindableObject&& other) noexcept:
		INotifyPropertyChanged(std::move(other)),
		_propertiesObservers(std::move(other._propertiesObservers)),
		_bindings(std::move(other._bindings)),
		_dataContext(std::move(other._dataContext))
	{}

	BindableObject::~BindableObject()
	{
		ClearBindings();
		Destroying(*this);
	}

	void BindableObject::SetDataContext(const Ref<INotifyPropertyChanged>& value, ValueSource source)
	{
		if(_dataContextSource > source || _dataContext == value)
		{
			return;
		}

		if(_dataContext)
		{
			ClearBindings();
		}

		_dataContextSource = source;
		_dataContext = value;

		if(_dataContext)
		{
			ApplyBindings();
		}

		NotifyPropertyChanged(DataContextProperty);
		OnDataContextChanged(value);
	}

	void BindableObject::AddPropertyChangedEventHandler(PropertyBase& property, PropertyChangedEventHandler handler)
	{
		_propertiesObservers.emplace_back(property, std::move(handler));
	}

	void BindableObject::RemovePropertyChangedEventHandler(PropertyBase& property, PropertyChangedEventHandler handler)
	{		
		Observer propertyObserver(property, std::move(handler));
		std::erase(_propertiesObservers, propertyObserver);
	}	

	void BindableObject::NotifyPropertyChanged(PropertyBase& property)
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

	void BindableObject::ApplyBindings()
	{
		for(auto& binding : _bindings)
		{
			if(!binding->IsApplied())
			{
				binding->Apply(*this);
			}
		}
	}

	void BindableObject::ClearBindings()
	{
		for(auto& binding : _bindings)
		{
			if(binding->IsApplied())
			{
				binding->Clear(*this);
			}
		}
	}
}
