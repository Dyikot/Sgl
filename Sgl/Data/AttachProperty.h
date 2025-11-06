#pragma once

namespace Sgl
{
	template<typename T>
	class AttachedProperty
	{
	public:
		using Value = T;
		using PropertyId = size_t;

		const PropertyId Id = _id++;
		const T DefaultValue = {};
	private:
		static inline PropertyId _id = 0;
	public:
		AttachedProperty() requires std::default_initializable<T> = default;
		AttachedProperty(const T& defaultValue): DefaultValue(defaultValue) {}
		AttachedProperty(const AttachedProperty&) = delete;
		AttachedProperty(AttachedProperty&&) = delete;
		~AttachedProperty() = default;
	};
}