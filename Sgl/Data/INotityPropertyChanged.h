#pragma once

#include "../Base/Event.h"
#include "Property.h"

namespace Sgl
{
	class INotifyPropertyChanged;

	using PropertyChangedEventHandler = EventHandler<INotifyPropertyChanged, PropertyBase&>;

	class INotifyPropertyChanged
	{
	public:
		virtual ~INotifyPropertyChanged() = default;

        virtual void AddPropertyChangedEventHandler(PropertyBase& property, PropertyChangedEventHandler handler) = 0;
        virtual void RemovePropertyChangedEventHandler(PropertyBase& property, PropertyChangedEventHandler handler) = 0;
	};
}