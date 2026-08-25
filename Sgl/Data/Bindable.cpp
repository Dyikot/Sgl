#include "Bindable.h"

namespace Sgl
{
	Bindable::Bindable(Bindable&& other) noexcept:
		_bindings(std::move(other._bindings)),
		_dataContext(std::move(other._dataContext))
	{}

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
			_dataContextSource = source;
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

		OnPropertyChanged(DataContextProperty);
		OnDataContextChanged(value);
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
