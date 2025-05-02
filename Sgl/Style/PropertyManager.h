#pragma once


#include <unordered_map>
#include "../Data/Any.h"
#include "PropertyId.h"

namespace Sgl
{
	template<typename T>
	struct StyleContext
	{

	};

	class PropertyManager
	{
	private:
		static inline std::unordered_map<PropertyId, Any> _properties;
	public:
		template<typename T>
		static inline const PropertyId& Register(std::string_view name)
		{
			auto [it, added] = _properties.emplace(
				PropertyId(name, typeid(T)), 
				CreateAny<StyleContext<T>>());
			
			if(!added)
			{
				throw std::runtime_error("Property with that name already exist");
			}

			return it->first;
		}
	};

}