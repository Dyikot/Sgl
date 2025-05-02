#pragma once

#include <assert.h>
#include "PropertyMap.h"
#include "PropertyManager.h"
#include "../Data/Delegates.h"
#include "Style.h"

namespace Sgl
{
	class StyleableObject
	{
	private:
		PropertyMap _properties;
	public:
		void SetStyle(const Style& style) {}
	protected:
		template<typename TValue, typename... TArgs>
		void AddProperty(const PropertyId& id, TArgs&&... args)
		{
			_properties.Add<TValue>(id, std::forward<TArgs>(args)...);
		}

		template<typename TValue>
		void SetProperty(const PropertyId& id, const TValue& value)
		{
			_properties[id].As<TValue>() = value;
		}

		template<typename TValue>
		const TValue& GetProperty(const PropertyId& id) const
		{
			return _properties.at(id).As<TValue>();
		}

		template<typename TValue>
		TValue& GetProperty(const PropertyId& id)
		{
			return _properties[id].As<TValue>();
		}
	};
}
