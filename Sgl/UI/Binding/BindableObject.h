#pragma once

#include <unordered_map>
#include <assert.h>
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

		template<typename TSource, typename TOwner, typename T, typename TInput = T>
		void Bind(BindableProperty<TOwner, T, TInput>& property, T TSource::* observer)
		{
			_observers[property.Id] = [observer](void* dataContext, const void* value)
			{
				auto source = static_cast<TSource*>(dataContext);
				const auto& inputValue = *static_cast<const TInput*>(value);
				std::invoke(observer, source) = inputValue;
			};
		}

		template<typename TSource, typename TOwner, typename T, typename TInput = T>
		void Bind(BindableProperty<TOwner, T, TInput>& property, void (TSource::*observer)(TInput))
		{
			_observers[property.Id] = [observer](void* dataContext, const void* value)
			{
				auto source = static_cast<TSource*>(dataContext);
				const auto& inputValue = *static_cast<const TInput*>(value);
				std::invoke(observer, source, inputValue);
			};
		}

		template<typename TOwner, typename T, typename TInput = T>
		void Unbind(BindableProperty<TOwner, T, TInput>& property)
		{
			_observers.erase(property.Id);
		}

		template<typename TOwner, typename T, typename TInput = T>
		Action<TInput> GetObserver(BindableProperty<TOwner, T, TInput>& property)
		{
			return [&property, owner = static_cast<TOwner*>(this)](TInput value)
			{
				std::invoke(property.PropertySetter, owner, value);
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
					assert(DataContext != nullptr);
					it->second(DataContext.get(), &value);
				}
			}
		}
	};
}