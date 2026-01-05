#pragma once

#include <vector>
#include <concepts>
#include "INotityPropertyChanged.h"
#include "DirectProperty.h"

namespace Sgl
{
	class ObservableObject : public INotifyPropertyChanged
	{
	private:
		struct Observer
		{
			std::reference_wrapper<AbstractPropertyBase> Property;
			PropertyChangedEventHandler Handler;

			bool operator==(const Observer& other) const
			{
				return Handler == other.Handler;
			}
		};

		std::vector<Observer> _observers;
	public:
		ObservableObject() = default;
		ObservableObject(const ObservableObject&) = default;
		ObservableObject(ObservableObject&&) = default;

		void AddPropertyChangedEventHandler(AbstractPropertyBase& property, PropertyChangedEventHandler handler) override;
		void RemovePropertyChangedEventHandler(AbstractPropertyBase& property, PropertyChangedEventHandler handler) override;
	protected:
		virtual void NotifyPropertyChanged(AbstractPropertyBase& property);

		template<typename TOwner, typename TValue, typename TField>
		bool SetProperty(DirectProperty<TOwner, TValue>& property, TField& field,
						 DirectProperty<TOwner, TValue>::Value value)
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
		bool SetProperty(DirectProperty<TOwner, TValue>& property,
						 DirectProperty<TOwner, TValue>::Value oldValue,
						 DirectProperty<TOwner, TValue>::Value newValue,
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