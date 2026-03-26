#pragma once

#include <vector>
#include <concepts>
#include "INotifyPropertyChanged.h"
#include "DirectProperty.h"

namespace Sgl
{
	/// <summary>
	/// Base class for objects that support property change notification.
	/// Provides helper methods for setting properties and raising PropertyChanged events.
	/// </summary>
	class ObservableObject : public INotifyPropertyChanged
	{
	public:
		ObservableObject() = default;
		ObservableObject(const ObservableObject&) = default;
		ObservableObject(ObservableObject&&) = default;

		/// <summary>
		/// Sets the property value and raises PropertyChanged if the value changed.
		/// </summary>
		/// <param name="property"> - the property metadata.</param>
		/// <param name="field"> - the backing field.</param>
		/// <param name="value"> - the new value.</param>
		/// <returns>True if the value changed, false otherwise.</returns>
		template<CProperty TProperty, typename TField>
		bool SetProperty(TProperty& property, TField& field, TProperty::Value value)
		{
			if(field == value)
			{
				return false;
			}

			field = value;
			NotifyPropertyChanged(property);

			return true;
		}

		/// <summary>
		/// Sets the property value with a custom change action and raises PropertyChanged if the value changed.
		/// </summary>
		/// <param name="property"> - the property metadata.</param>
		/// <param name="oldValue"> - the old value.</param>
		/// <param name="newValue"> - the new value.</param>
		/// <param name="changed"> - action to perform when the value changes.</param>
		/// <returns>True if the value changed, false otherwise.</returns>
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
			NotifyPropertyChanged(property);

			return true;
		}
	protected:
		/// <summary>
		/// Raises the PropertyChanged event for the specified property.
		/// </summary>
		/// <param name="property"> - the property that changed.</param>
		virtual void NotifyPropertyChanged(PropertyBase& property)
		{
			PropertyChanged.Invoke(*this, property);
		}
	};
}