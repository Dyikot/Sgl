#pragma once

#include "../../Base/Observable/IObserver.h"
#include "BindableProperty.h"

namespace Sgl
{
	template<typename TOwner, typename T, typename TInput = T>
	class PropertyObserver: public IObserver<TInput>
	{
	public:
		using BindableProperty = BindableProperty<TOwner, T, TInput>;
	private:
		TOwner& _owner;
		BindableProperty& _property;
	public:
		PropertyObserver(TOwner& owner, BindableProperty& property):
			_owner(owner),
			_property(property)
		{}

		void OnNext(TInput value) override
		{
			std::invoke(_property.PropertySetter, _owner, value);
		}
	};
}