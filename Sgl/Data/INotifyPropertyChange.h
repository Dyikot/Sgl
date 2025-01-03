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

	class INotifyPropertyChange
	{
	public:
		static constexpr auto GetMember(const std::string_view name) {}

		virtual ~INotifyPropertyChange() = default;

		virtual Event<PropertyChangedEventHanlder>& GetPropertyChangedEvent() = 0;
	};
}