#pragma once

#include "../Data/Binding/BindableObject.h"
#include "../Data/Binding/PropertyMap.h"

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