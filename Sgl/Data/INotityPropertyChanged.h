#pragma once

#include "../Base/Event.h"
#include "AbstractProperty.h"

namespace Sgl
{
	class INotifyPropertyChanged;

	using PropertyChangedEventHandler = EventHandler<INotifyPropertyChanged, AbstractPropertyBase&>;

	class INotifyPropertyChanged
	{
	public:
		virtual ~INotifyPropertyChanged() = default;

        virtual void AddPropertyChangedEventHandler(AbstractPropertyBase& property, PropertyChangedEventHandler handler) = 0;
        virtual void RemovePropertyChangedEventHandler(AbstractPropertyBase& property, PropertyChangedEventHandler handler) = 0;
	};
}