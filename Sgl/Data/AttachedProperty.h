#pragma once

#include <concepts>
#include "Property.h"

namespace Sgl
{
	template<typename T>
	class AttachedProperty : public PropertyBase
	{
	public:
		const T DefaultValue = {};
	public:
		AttachedProperty() requires std::default_initializable<T> = default;
		AttachedProperty(const T& defaultValue): DefaultValue(defaultValue) {}
		AttachedProperty(const AttachedProperty&) = delete;
		AttachedProperty(AttachedProperty&&) = delete;
	};
}