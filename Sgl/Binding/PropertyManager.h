#pragma once

#include <unordered_set>
#include <unordered_map>
#include <string>
#include <typeinfo>
#include <stdexcept>
#include "PropertyId.h"

namespace Sgl
{
	class PropertyManager
	{
	protected:
		static inline std::unordered_map<PropertyId, std::string> _propertyTypeMap;
		static inline std::unordered_set<std::string> _propertyNames;
	public:
		template<typename T>
		static const PropertyId Register(std::string&& name)
		{
			auto [_, isInserted] = _propertyNames.insert(std::move(name));

			if(isInserted)
			{
				PropertyId id = _propertyNames.size() - 1;
				_propertyTypeMap.emplace(id, typeid(T).name());
				return id;
			}
			else
			{
				throw std::invalid_argument("Property with this name already exist\n");
			}
		}

		static std::string_view GetTypeNameBy(PropertyId id)
		{
			return _propertyTypeMap.at(id);
		}
	};
}