#pragma once

#include "Property.h"
#include "../Base/Event.h"
#include "../Base/Ref.h"

namespace Sgl
{
	//! @brief Base class for objects that support property change notification
	class ObservableObject : public RefCounted
	{
	public:
		using PropertyChangedEventHandler = EventHandler<ObservableObject, PropertyBase&>;
	public:
		ObservableObject() = default;

		//! @brief Occurs when a property value changes
		Event<PropertyChangedEventHandler> PropertyChanged;

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
}