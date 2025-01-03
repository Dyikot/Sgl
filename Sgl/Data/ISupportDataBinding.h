#pragma once

#include "../Events/Event.h"
#include <string>

namespace Sgl
{
	struct PropertyChangedEventArgs: public EventArgs
	{
		std::string MemberName;
	};

	using PropertyChangedEventHanlder = EventHandler<void, PropertyChangedEventArgs>;

	class Component;

	class ISupportDataBinding
	{
	public:
		virtual ~ISupportDataBinding() = default;

		virtual Event<PropertyChangedEventHanlder>& GetPropertyChangedEvent() = 0;
		virtual void SetBindings(Component& component) = 0;
	};
}