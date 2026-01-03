#pragma once

#include "../Base/Event.h"
#include "SglProperty.h"

namespace Sgl
{
	class INotifyPropertyChanged;

	using PropertyChangedEventHandler = EventHandler<INotifyPropertyChanged, SglPropertyBase&>;

	class INotifyPropertyChanged
	{
	public:
		virtual ~INotifyPropertyChanged() = default;

        virtual void AddPropertyChangedEventHandler(SglPropertyBase& property, PropertyChangedEventHandler handler) = 0;
        virtual void RemovePropertyChangedEventHandler(SglPropertyBase& property, PropertyChangedEventHandler handler) = 0;
	};
}