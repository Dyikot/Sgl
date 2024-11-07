#include "Property.h"


namespace Sgl
{
	std::string_view PropertyManager::GetTypeNameBy(const PropertyId id)
	{
		return _typeNameMap.at(id);
	}
}