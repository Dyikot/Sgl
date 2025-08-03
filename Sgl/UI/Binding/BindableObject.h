#pragma once

#include <unordered_map>
#include "BindableProperty.h"
#include "PropertyObserver.h"
#include "../../Base/Delegate.h"

namespace Sgl
{
	class BindableObject
	{
	private:
		using PropertyId = size_t;

		std::unordered_map<PropertyId, void*> _observers;
	public:
		template<typename TOwner, typename T, typename TInput = T>
		void Subscribe(BindableProperty<TOwner, T, TInput>& property, IObserver<TInput>& observer)
		{
			_observers[property.Id] = &observer;
		}

		template<typename TOwner, typename T, typename TInput = T>
		void Unsubscribe(BindableProperty<TOwner, T, TInput>& property)
		{
			_observers.erase(property.Id);
		}

		template<typename TOwner, typename T, typename TInput = T>
		Shared<IObserver<TInput>> GetObserver(BindableProperty<TOwner, T, TInput>& property)
		{
			return NewShared<PropertyObserver<TOwner, TInput>>(static_cast<TOwner&>(*this), property.PropertySetter);
		}

		template<typename TOwner, typename T, typename TInput = T>
		Action<TInput> GetActionObserver(BindableProperty<TOwner, T, TInput>& property)
		{
			return [&property, owner = static_cast<TOwner*>(this)](TInput value)
			{
				(owner->*property.PropertySetter)(value);
			};
		}
	protected:
		template<typename TOwner, typename T, typename TInput = T>
		void SetProperty(BindableProperty<TOwner, T, TInput>& property, T& field, const std::remove_reference_t<TInput>& value)
		{
			if(field != value)
			{
				field = value;

				if(auto it = _observers.find(property.Id); it != _observers.end())
				{
					static_cast<IObserver<TInput>*>(it->second)->OnNext(value);
				}
			}
		}
	};
}