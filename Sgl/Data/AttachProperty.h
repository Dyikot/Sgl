#pragma once

#include "SglProperty.h"

namespace Sgl
{
	template<typename T>
	class AttachedProperty : public SglProperty
	{
	public:
		using Value = T;

		const T DefaultValue = {};
	public:
		AttachedProperty() requires std::default_initializable<T> = default;
		AttachedProperty(const T& defaultValue): DefaultValue(defaultValue) {}
		AttachedProperty(const AttachedProperty&) = delete;
		AttachedProperty(AttachedProperty&&) = delete;
		~AttachedProperty() = default;
	};
}