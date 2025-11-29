#pragma once

namespace Sgl
{
	using PropertyId = size_t;

	class SglProperty
	{
	private:
		static inline PropertyId _nextId = 0;
	public:
		const PropertyId Id = _nextId++;
	public:
		SglProperty() = default;
		SglProperty(const SglProperty&) = default;
		SglProperty(SglProperty&&) = default;
	};
}