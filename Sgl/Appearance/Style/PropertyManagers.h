#pragma once

#include "GenericPropertyManager.h"
#include "Setters.h"

namespace Sgl
{
	class PropertyManager: public GenericPropertyManager<PropertyId>
	{
	public:
		using Base = GenericPropertyManager<PropertyId>;
	private:
		static inline PropertySetterMap _defaultValuesMap;
	public:
		template<typename T>
		static const PropertyId Register(std::string&& name, auto&& defaultValue)
		{
			auto id = Base::Register<T>(std::move(name));
			_defaultValuesMap.Add<T>(id, std::forward<decltype(defaultValue)>(defaultValue));
			return id;
		}

		template<typename T>
		static const PropertyId Register(std::string&& name)
		{
			return Register<T>(std::move(name), T());
		}

		static inline const PropertySetterMap& DefaultValuesMap = _defaultValuesMap;		
	};

	using EventManager = GenericPropertyManager<EventId>;
}