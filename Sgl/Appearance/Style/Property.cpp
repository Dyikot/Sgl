#include "Property.h"


namespace Sgl
{
	std::string_view PropertyManager::GetTypeNameOf(const PropertyId id)
	{
		return _typeNameMap.at(id);
	}
}