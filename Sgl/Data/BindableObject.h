#pragma once

#include <unordered_map>
#include "BindingMode.h"
#include "ObservableObject.h"
#include "../Base/Ref.h"

namespace Sgl
{
	class BindableObject
	{
	public:
		Ref<void> DataContext;
	private:
		std::unordered_map<size_t, Action<void*, const void*>> _observers;
	public:
		BindableObject() = default;
		BindableObject(const BindableObject& other);
		BindableObject(BindableObject&& other) noexcept;
		virtual ~BindableObject() = default;

		template<typename TTarget, typename TValue, typename TField, typename TSource>
		void Bind(ObservableProperty<TTarget, TValue>& property, TField TSource::* observer)
		{
			_observers[property.Id] = [observer](void* dataContext, const void* value)
			{
				auto source = As<TSource>(dataContext);
				const auto& targetValue = AsValue<TValue>(value);
				std::invoke(observer, source) = targetValue;
			};
		}

		template<typename TTarget, typename TValue, typename TSource>
		void Bind(ObservableProperty<TTarget, TValue>& property, void (TSource::*observer)(TValue))
		{
			_observers[property.Id] = [observer](void* dataContext, const void* value)
			{
				auto source = As<TSource>(dataContext);
				const auto& targetValue = AsValue<TValue>(value);
				std::invoke(observer, source, targetValue);
			};
		}

		template<typename TTarget, typename TSource, typename TValue>
		void Bind(ObservableProperty<TTarget, TValue>& targetProperty, 
				  ObservableProperty<TSource, TValue>& sourceProperty,
				  BindingMode bindingMode = BindingMode::OneWay)
		{
			if(bindingMode != BindingMode::OneWay)
			{
				Bind(targetProperty, sourceProperty.Setter);
			}

			if(bindingMode == BindingMode::OneWayToSource)
			{
				return;
			}

			if constexpr(std::derived_from<TSource, ObservableObject>)
			{
				if(DataContext == nullptr)
				{
					throw std::exception("Cannot bind to a nullable source");
				}

				auto source = DataContext.GetAs<TSource>();
				auto target = As<TTarget>(this);

				auto currentValue = std::invoke(sourceProperty.Getter, source);
				std::invoke(targetProperty.Setter, target, currentValue);

				auto observer = [&targetProperty, target](const void* value)
				{
					const auto& sourceValue = AsValue<TValue>(value);
					std::invoke(targetProperty.Setter, target, sourceValue);
				};

				std::invoke(&ObservableObject::SetObserver, source, sourceProperty.Id, observer);
			}
		}

		template<typename TTarget, typename TValue>
		void ClearBinding(ObservableProperty<TTarget, TValue>& property)
		{
			_observers.erase(property.Id);
		}

		template<std::derived_from<ObservableObject> TSource, typename TValue>
		void ClearBinding(ObservableProperty<TSource, TValue>& property)
		{
			if(DataContext == nullptr)
			{
				throw std::exception("Cannot remove a binding from a nullable source");
			}

			auto source = DataContext.GetAs<TSource>();
			std::invoke(&ObservableObject::RemoveObserver, source, property.Id);
		}

		template<typename TTarget, typename TSource, typename TValue>
		void ClearBinding(ObservableProperty<TTarget, TValue>& targetProperty,
						  ObservableProperty<TSource, TValue>& sourceProperty)
		{
			ClearBinding(targetProperty);
			ClearBinding(sourceProperty);
		}

		template<typename TOwner, typename TValue>
		auto GetObserver(ObservableProperty<TOwner, TValue>& property)
		{
			return [&property, owner = static_cast<TOwner*>(this)](TValue value)
			{
				std::invoke(property.Setter, owner, value);
			};
		}
	protected:
		template<typename TOwner, typename TValue, typename TField>
		void SetProperty(ObservableProperty<TOwner, TValue>& property, TField& field, 
						 ObservableProperty<TOwner, TValue>::Value value)
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

					it->second(DataContext.Get(), &value);
				}
			}
		}
	private:
		template<typename T>
		T* As(void* value)
		{
			return static_cast<T*>(value);
		}

		template<typename T>
		const T& AsValue(const void* value)
		{
			return *static_cast<const std::decay_t<T>*>(value);
		}
	};
}