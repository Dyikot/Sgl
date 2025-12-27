#pragma once

#include "../Base/Event.h"
#include "SglProperty.h"

namespace Sgl
{
	class INotityPropertyChanged;

	using PropertyChangedEventHandler = EventHandler<INotityPropertyChanged, SglPropertyBase&>;

	class INotityPropertyChanged
	{
	public:
		virtual ~INotityPropertyChanged() = default;

        virtual void AddPropertyChangedEventHandler(SglPropertyBase& property, PropertyChangedEventHandler handler) = 0;
        virtual void RemovePropertyChangedEventHandler(SglPropertyBase& property, PropertyChangedEventHandler handler) = 0;
	};
}