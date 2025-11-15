#pragma once

namespace Sgl
{
	class SglProperty
	{
	public:
		const size_t Id;
	public:
		SglProperty();
		SglProperty(const SglProperty&) = default;
		SglProperty(SglProperty&&) = default;
	private:
		static size_t GenerateUniquePropertyId();
	};
}