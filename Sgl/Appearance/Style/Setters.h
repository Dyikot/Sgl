#pragma once

#include "../../Collections/SetterMap.h"
#include "Property.h"

namespace Sgl
{
	using PropertySetterMap = SetterMap<PropertyId>;

	class EventSetterMap: public SetterMap<EventId>
	{
	public:
		template<typename TEventHanlder>
		bool TrySetEvent(EventId id, Event<TEventHanlder>& event) const
		{
			if(Contains(id))
			{
				event += At(id).As<TEventHanlder>();
				return true;
			}

			return false;
		}
	};
}