#pragma once

#include <concepts>

namespace Sgl
{
	class AttachableObject;

	class AttachedPropertyBase
	{
	public:
		AttachedPropertyBase() = default;
		AttachedPropertyBase(const AttachedPropertyBase&) = delete;
		AttachedPropertyBase(AttachedPropertyBase&&) = delete;

		friend bool operator==(const AttachedPropertyBase& left, const AttachedPropertyBase& right)
		{
			return &left == &right;
		}
	};

	template<typename T>
	class AttachedProperty : public AttachedPropertyBase
	{
	public:
		using Value = T;

		const T DefaultValue = {};
	public:
		AttachedProperty() requires std::default_initializable<T> = default;
		AttachedProperty(const T& defaultValue): DefaultValue(defaultValue) {}
		AttachedProperty(const AttachedProperty&) = delete;
		AttachedProperty(AttachedProperty&&) = delete;
	};
}