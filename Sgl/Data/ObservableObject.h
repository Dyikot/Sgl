#pragma once

#include <vector>
#include <concepts>
#include "INotifyPropertyChanged.h"
#include "DirectProperty.h"

namespace Sgl
{
	class ObservableObject : public INotifyPropertyChanged
	{
	public:
		ObservableObject() = default;
		ObservableObject(const ObservableObject&) = default;
		ObservableObject(ObservableObject&&) = default;

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
		virtual void NotifyPropertyChanged(PropertyBase& property)
		{
			PropertyChanged.Invoke(*this, property);
		}
	};
}