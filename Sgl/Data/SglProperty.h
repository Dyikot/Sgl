#pragma once

namespace Sgl
{
	using PropertyId = size_t;

	class SglProperty
	{
	public:
		const PropertyId Id;
	public:
		SglProperty();
		SglProperty(const SglProperty&) = default;
		SglProperty(SglProperty&&) = default;
	private:
		static PropertyId GenerateUniquePropertyId();
	};
}