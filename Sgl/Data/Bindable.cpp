#include "Bindable.h"

namespace Sgl
{
	Bindable::~Bindable()
	{
		ClearBindings();
	}

	void Bindable::SetValueSource(ValueSource valueSource)
	{
		_valueSource = valueSource;
	}

	void Bindable::SetDataContext(const Ref<ObservableObject>& value)
	{
		if(_dataContextSource > _valueSource)
		{
			return;
		}

		if(_dataContext == value)
		{
			_dataContextSource = _valueSource;
			return;
		}

		if(_dataContext)
		{
			ClearBindings();
		}

		_dataContext = value;
		_dataContextSource = _valueSource;

		if(_dataContext)
		{
			ApplyBindings();
		}

		OnPropertyChanged(DataContextProperty);
	}

	void Bindable::ClearValue(PropertyBase& property)
	{
		_valueStorage.Remove(&property);
		OnPropertyChanged(property);
	}

	void Bindable::ClearAllValues()
	{
		std::vector<PropertyBase*> properties = _valueStorage.Keys();
		_valueStorage.Clear();

		for(auto property : properties)
		{
			OnPropertyChanged(*property);
		}
	}

	void Bindable::OnPropertyChanged(PropertyBase& property)
	{
		ObservableObject::OnPropertyChanged(property);

		if(property == DataContextProperty)
		{
			OnDataContextChanged(_dataContext);
		}
	}

	void Bindable::ClearBinding(PropertyBase& targetProperty)
	{
		auto it = std::ranges::find_if(_bindings, [&targetProperty](auto& binding)
		{
			return binding->GetTarget() == targetProperty;
		});

		if(it != _bindings.end())
		{
			(*it)->Clear(*this);
			_bindings.erase(it);
		}
	}

	void Bindable::ApplyBindings()
	{
		for(auto& binding : _bindings)
		{
			if(!binding->IsApplied())
			{
				binding->Apply(*this);
			}
		}
	}

	void Bindable::ClearBindings()
	{
		for(auto& binding : _bindings)
		{
			if(binding->IsApplied())
			{
				binding->Clear(*this);
			}
		}
	}

	ValueSourceScope::ValueSourceScope(Bindable& target, ValueSource valueSource):
		_target(target),
		_currentSource(target.GetValueSource())
	{
		_target.SetValueSource(valueSource);
	}

	ValueSourceScope::ValueSourceScope(const Ref<Bindable>& target, ValueSource valueSource):
		ValueSourceScope(target.GetValue(), valueSource)
	{}

	ValueSourceScope::~ValueSourceScope()
	{
		_target.SetValueSource(_currentSource);
	}
}
