#pragma once

#include <unordered_map>
#include "../../Base/SmartPointers.h"
#include "../../Base/Delegate.h"
#include "../../Base/Observable/ObservableObject.h"
#include "BindingMode.h"

namespace Sgl
{
	class BindableObject
	{
	public:
		Shared<void> DataContext;
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
				auto source = static_cast<TSource*>(dataContext);
				std::invoke(observer, source) = *static_cast<const TValue*>(value);
			};
		}

		template<typename TTarrget, typename TValue, typename TSource>
		void Bind(ObservableProperty<TTarrget, TValue>& property, void (TSource::*observer)(TValue))
		{
			_observers[property.Id] = [observer](void* dataContext, const void* value)
			{
				auto source = static_cast<TSource*>(dataContext);
				std::invoke(observer, source, *static_cast<const TValue*>(value));
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

				auto source = static_cast<TSource*>(DataContext.get());
				auto observer = [&targetProperty, target = static_cast<TTarget*>(this)](const void* value)
				{
					std::invoke(targetProperty.Setter, target, *static_cast<const TValue*>(value));
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

			auto source = static_cast<TSource*>(DataContext.get());

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

					it->second(DataContext.get(), &value);
				}
			}
		}
	};
}