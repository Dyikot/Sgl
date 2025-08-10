#pragma once

#include <unordered_map>
#include <cassert>
#include "BindableProperty.h"
#include "../../Base/SmartPointers.h"
#include "../../Base/Delegate.h"

namespace Sgl
{
	class BindableObject
	{
	public:
		Shared<void> DataContext;
	private:
		using PropertyId = size_t;

		std::unordered_map<PropertyId, Action<void*, const void*>> _observers;
	public:
		BindableObject() = default;
		BindableObject(const BindableObject& other);
		BindableObject(BindableObject&& other) noexcept;
		virtual ~BindableObject() = default;

		template<typename TSource, typename TOwner, typename TField, typename TValue>
		void Bind(BindableProperty<TOwner, TValue>& property, TField TSource::* observer)
		{
			_observers[property.Id] = [observer](void* dataContext, const void* value)
			{
				auto source = static_cast<TSource*>(dataContext);
				std::invoke(observer, source) = *static_cast<const TValue*>(value);
			};
		}

		template<typename TSource, typename TOwner, typename TValue>
		void Bind(BindableProperty<TOwner, TValue>& property, void (TSource::*observer)(TValue))
		{
			_observers[property.Id] = [observer](void* dataContext, const void* value)
			{
				auto source = static_cast<TSource*>(dataContext);
				std::invoke(observer, source, *static_cast<const TValue*>(value));
			};
		}

		template<typename TOwner, typename TValue>
		void Unbind(BindableProperty<TOwner, TValue>& property)
		{
			_observers.erase(property.Id);
		}

		template<typename TOwner, typename TValue>
		Action<TValue> GetObserver(BindableProperty<TOwner, TValue>& property)
		{
			return [&property, owner = static_cast<TOwner*>(this)](TValue value)
			{
				std::invoke(property.PropertySetter, owner, value);
			};
		}
	protected:
		template<typename TOwner, typename TField, typename TValue>
		void SetProperty(BindableProperty<TOwner, TValue>& property, TField& field, 
						 BindableProperty<TOwner, TValue>::Value value)
		{
			if(field != value)
			{
				field = value;

				if(auto it = _observers.find(property.Id); it != _observers.end())
				{
					if(DataContext == nullptr)
					{
						throw std::exception("DataContext is null");
					}

					it->second(DataContext.get(), &value);
				}
			}
		}
	};
}