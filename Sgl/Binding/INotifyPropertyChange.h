#pragma once

#include "../Events/Event.h"
#include <string>

namespace Sgl
{
	struct PropertyChangedEventArgs: public EventArgs
	{
		std::string PropertyName;
	};

	using PropertyChangedEventHandler = EventHandler<void, PropertyChangedEventArgs>;

	class INotifyPropertyChange
	{
	public:
		virtual ~INotifyPropertyChange() = default;

		virtual Event<PropertyChangedEventHandler>& GetPropertyChangedEvent() = 0;
	};
}