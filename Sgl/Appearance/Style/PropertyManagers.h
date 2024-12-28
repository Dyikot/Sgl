#pragma once

#include "UniquePropertyManager.h"
#include "Setters.h"

namespace Sgl
{
	class PropertyManager: public UniquePropertyManager<PropertyId>
	{
	public:
		using Base = UniquePropertyManager<PropertyId>;
	private:
		static inline PropertySetterMap _defaultProperties;
	public:
		template<typename T>
		static const PropertyId Register(std::string&& name, auto&& defaultValue)
		{
			auto id = Base::Register<T>(std::move(name));
			_defaultProperties.Add<T>(id, std::forward<decltype(defaultValue)>(defaultValue));
			return id;
		}

		template<typename T>
		static const PropertyId Register(std::string&& name)
		{
			return Register<T>(std::move(name), T());
		}

		static const PropertySetterMap& GetDefaultProperties() { return _defaultProperties; }
	};

	using EventManager = UniquePropertyManager<EventId>;
}