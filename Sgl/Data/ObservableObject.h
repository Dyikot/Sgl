#pragma once

#include "Property.h"
#include "../Base/Event.h"
#include "../Base/Ref.h"

namespace Sgl
{
	template<CProperty TProperty, typename TObserver>
	struct PropertyObserver;

	//! @brief Base class for objects that support property change notification
	class ObservableObject : public RefCounted
	{
	public:
		using PropertyChangedEventHandler = EventHandler<ObservableObject, PropertyBase&>;
	public:
		ObservableObject() = default;

		//! @brief Occurs when a property value changes
		Event<PropertyChangedEventHandler> PropertyChanged;

		//! @brief Subscribes an observer to be notified when a specific property changes
		//! @param property Reference to the property to observe
		//! @param observer The callback to invoke when the property changes
		template<CProperty TProperty, typename TObserver>
		void AddObserver(TProperty& property, TObserver observer)
		{
			PropertyChanged += PropertyObserver(property, std::move(observer));
		}

		//! @brief Unsubscribes an observer from a specific property
		//! @param property Reference to the property to stop observing
		//! @param observer The callback to remove
		template<CProperty TProperty, typename TObserver>
		void RemoveObserver(TProperty& property, TObserver observer)
		{
			PropertyChanged -= PropertyObserver(property, std::move(observer));
		}

	protected:
		//! @brief Sets the property value and raises PropertyChanged if the value changed
		//! @param property Reference to property
		//! @param field The backing field
		//! @param value The new value
		//! @return True if the value changed, false otherwise
		template<CProperty TProperty, typename TField>
		bool SetProperty(TProperty& property, TField& field, TProperty::Value value)
		{
			if(field == value)
			{
				return false;
			}

			field = value;
			OnPropertyChanged(property);

			return true;
		}

		//! @brief Sets the property value with a custom change action and raises PropertyChanged if the value changed
		//! @param property Reference to property
		//! @param oldValue The old value
		//! @param newValue The new value
		//! @param changed Action to perform when the value changes
		//! @return True if the value changed, false otherwise
		template<CProperty TProperty, typename TField>
		bool SetProperty(TProperty& property,
						 TProperty::Value oldValue,
						 TProperty::Value newValue,
						 Action<typename TProperty::Value>& changed)
		{
			if(oldValue == newValue)
			{
				return false;
			}

			changed(newValue);
			OnPropertyChanged(property);

			return true;
		}

		virtual void OnPropertyChanged(PropertyBase& property)
		{
			PropertyChanged.Invoke(*this, property);
		}
	};

	//! @brief A wrapper that binds an observer callback to a specific property.
	//! It acts as an event handler for the PropertyChanged event. When invoked, 
	//! it checks if the raised property matches the bound property. If it does, it extracts 
	//! the new value and passes it to the observer callback.
	template<CProperty TProperty, typename TObserver>
	struct PropertyObserver
	{
	public:
		using Owner = TProperty::Owner;
		using Value = TProperty::Value;

		static_assert(std::invocable<TObserver, Value>);

	public:
		TProperty& property;
		TObserver observer;

		void operator()(ObservableObject& sender, PropertyBase& p) const
		{
			if(property == p)
			{
				observer(property.InvokeGetter(static_cast<TProperty::Owner&>(sender)));
			}
		}

		bool operator==(const PropertyObserver& other) const
		{
			return property == other.property && observer == other.observer;
		}
	};
}