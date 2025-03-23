#pragma once

#include "../Data/BindableObject.h"
#include "../Data/PropertyMap.h"

namespace Sgl
{
	class Style
	{
	public:
		PropertyMap Setters;
	public:
		Style() = default;
		Style(const Style& basedOn) = default;
		Style(Style&& style) = default;
	};
}