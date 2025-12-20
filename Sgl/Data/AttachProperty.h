#pragma once

#include <concepts>

namespace Sgl
{
	class AttachedPropertyBase
	{
	private:
		static inline uint32_t _nextId = 0;
	public:
		const uint32_t Id = _nextId++;
	public:
		AttachedPropertyBase() = default;
		AttachedPropertyBase(const ObservablePropertyBase&) = delete;
		AttachedPropertyBase(ObservablePropertyBase&&) = delete;

		friend class AttachableObject;
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