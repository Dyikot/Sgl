#pragma once

#include <format>
#include <cassert>
#include "../../Events/Event.h"
#include "PropertyId.h"
#include "../Any.h"

namespace Sgl
{
	class PropertyMap: public std::unordered_map<PropertyId, Any>
	{
	public:
		void Add(const PropertyId& id, Any&& object)
		{
			assert(object.Is(id.Type));

			emplace(id, std::move(object));
		}

		void Add(const PropertyId& id, const Any& object)
		{
			assert(object.Is(id.Type));

			emplace(id, object);
		}

		template<typename TValue, typename... TArgs>
		void Add(const PropertyId& id, TArgs&&... args)
		{
			Add(id, Any::New<TValue>(std::forward<TArgs>(args)...));
		}

		template<typename TValue>
		bool TryGetValue(const PropertyId& id, TValue& value) const
		{
			if(contains(id))
			{
				value = at(id).As<TValue>();
				return true;
			}

			return false;
		}
	};	
}