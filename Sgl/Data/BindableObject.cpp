#include "BindableObject.h"

namespace Sgl
{
	BindableObject::BindableObject(const BindableObject& other):
		_dataContext(other._dataContext)
	{}

	BindableObject::BindableObject(BindableObject&& other) noexcept:
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

	void BindableObject::NotifyPropertyChanged(PropertyBase& property)
	{
		PropertyChanged(*this, property);
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
