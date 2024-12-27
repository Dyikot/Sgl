#pragma once

#include "../../Collections/SetterMap.h"

namespace Sgl
{
	class Style
	{
	public:
		SetterMap<PropertyId> Setters;
		SetterMap<EventId> EventSetters;
	public:
		Style() = default;
		Style(const Style& basedOn) = default;
		Style(Style&& style) = default;
	};
}