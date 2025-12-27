#pragma once

#include <vector>
#include <concepts>
#include "INotityPropertyChanged.h"

namespace Sgl
{
	class ObservableObject : public INotityPropertyChanged
	{
	private:
		struct Observer
		{
			std::reference_wrapper<SglPropertyBase> Property;
			PropertyChangedEventHandler Handler;

			bool operator==(const Observer&) const = default;
		};

		std::vector<Observer> _observers;
	public:
		ObservableObject() = default;
		ObservableObject(const ObservableObject&) = default;
		ObservableObject(ObservableObject&&) = default;

		void AddPropertyChangedEventHandler(SglPropertyBase& property, PropertyChangedEventHandler handler) override;
		void RemovePropertyChangedEventHandler(SglPropertyBase& property, PropertyChangedEventHandler handler) override;
	protected:
		virtual void NotifyPropertyChanged(SglPropertyBase& property);

		template<typename TOwner, typename TValue, typename TField>
		bool SetProperty(SglProperty<TOwner, TValue>& property, TField& field,
						 SglProperty<TOwner, TValue>::Value value)
		{
			if(field == value)
			{
				return false;
			}

			field = value;
			NotifyPropertyChanged(property);

			return true;
		}

		template<typename TOwner, typename TValue>
		bool SetProperty(SglProperty<TOwner, TValue>& property,
						 SglProperty<TOwner, TValue>::Value oldValue,
						 SglProperty<TOwner, TValue>::Value newValue,
						 Action<TValue>& changed)
		{
			if(oldValue == newValue)
			{
				return false;
			}

			changed(newValue);
			NotifyPropertyChanged(property);

			return true;
		}
	};
}