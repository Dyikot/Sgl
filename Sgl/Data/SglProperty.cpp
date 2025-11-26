#include "SglProperty.h"

namespace Sgl
{
	SglProperty::SglProperty():
		Id(GenerateUniquePropertyId())
	{}

	PropertyId SglProperty::GenerateUniquePropertyId()
	{
		static PropertyId lastId = 0;
		return lastId++;
	}
}
