#pragma once

#include "../Base/Event.h"
#include "Property.h"

namespace Sgl
{
	class INotifyPropertyChanged
	{
	public:
		using PropertyChangedEventHandler = EventHandler<INotifyPropertyChanged, PropertyBase&>;
	public:
		virtual ~INotifyPropertyChanged() = default;

		Event<PropertyChangedEventHandler> PropertyChanged;
	};
}