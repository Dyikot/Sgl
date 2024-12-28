#pragma once

#include "Setters.h"

namespace Sgl
{
	class Style
	{
	public:
		PropertySetterMap Setters;
		EventSetterMap EventSetters;
	public:
		Style() = default;
		Style(const Style& basedOn) = default;
		Style(Style&& style) = default;
	};
}