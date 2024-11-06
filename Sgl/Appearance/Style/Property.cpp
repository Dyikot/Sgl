#include "Property.h"


namespace Sgl
{
	std::string_view PropertyManager::GetTypeNameBy(const PropertyId id)
	{
		return _idToTypeNameMap.at(id);
	}
}