#pragma once

#include "../Base/Event.h"
#include "Property.h"

namespace Sgl
{
	/// <summary>
	/// Defines an interface for objects that notify subscribers when a property changes.
	/// This is the foundation of the data binding system.
	/// </summary>
	class INotifyPropertyChanged
	{
	public:
		using PropertyChangedEventHandler = EventHandler<INotifyPropertyChanged, PropertyBase&>;
	public:
		virtual ~INotifyPropertyChanged() = default;

		Event<PropertyChangedEventHandler> PropertyChanged;
	};
}