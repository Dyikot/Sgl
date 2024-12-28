#pragma once

#include "../../Collections/SetterMap.h"
#include "Property.h"

namespace Sgl
{
	using PropertySetterMap = SetterMap<PropertyId>;

	class EventSetterMap: public SetterMap<EventId>
	{
	public:
		template<typename THanlder>
		bool TrySetEvent(EventId id, Event<THanlder>& event) const
		{
			if(Contains(id))
			{
				event += At(id).As<THanlder>();
				return true;
			}

			return false;
		}
	};
}