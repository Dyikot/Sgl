#include "Bindable.h"

namespace Sgl
{
	Bindable::~Bindable()
	{
		ClearBindings();
	}

	void Bindable::SetDataContext(const Ref<ObservableObject>& value, ValueSource source)
	{
		if(_dataContextSource > source)
		{
			return;
		}

		if(_dataContext == value)
		{
			if(source < ValueSource::PseudoClass)
			{
				_dataContextSource = source;
			}

			return;
		}

		if(_dataContext)
		{
			ClearBindings();
		}

		_dataContext = value;
		_dataContextSource = source;

		if(_dataContext)
		{
			ApplyBindings();
		}

		OnPropertyChanged(DataContextProperty);
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
}
