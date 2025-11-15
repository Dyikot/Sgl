#include "SglProperty.h"

namespace Sgl
{
	SglProperty::SglProperty():
		Id(GenerateUniquePropertyId())
	{}

	size_t SglProperty::GenerateUniquePropertyId()
	{
		static size_t counter = 0;
		return counter++;
	}
}
