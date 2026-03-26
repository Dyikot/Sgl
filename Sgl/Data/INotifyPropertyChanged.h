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
		/// <summary>
		/// Event handler type for property changed notifications.
		/// </summary>
		using PropertyChangedEventHandler = EventHandler<INotifyPropertyChanged, PropertyBase&>;
	public:
		virtual ~INotifyPropertyChanged() = default;

		/// <summary>
		/// Event raised when a property value changes.
		/// Subscribers receive the property that changed.
		/// </summary>
		Event<PropertyChangedEventHandler> PropertyChanged;
	};
}