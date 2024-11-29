#pragma once

#include <unordered_set>
#include <unordered_map>
#include <string>
#include <typeinfo>
#include <stdexcept>
#include <functional>

namespace Sgl
{
	using PropertyId = size_t;	

	class PropertyManager
	{
	private:
		static inline std::unordered_map<PropertyId, std::string> _propertyIdMap = {};
		static inline std::unordered_set<std::string> _propertiesNames = {};
	public:
		template<typename T>
		static const PropertyId Register(std::string&& name)
		{
			auto [_, isInserted] = _propertiesNames.insert(std::move(name));
			
			if(isInserted)
			{
				PropertyId id = _propertiesNames.size() - 1;
				_propertyIdMap.emplace(id, typeid(T).name());
				return id;
			}
			else
			{
				throw std::invalid_argument("Property with this name already exist\n");
			}
		}
		static std::string_view GetTypeNameOf(const PropertyId id)
		{
			return _propertyIdMap.at(id);
		}
	};	
}