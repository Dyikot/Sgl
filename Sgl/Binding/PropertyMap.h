#pragma once

#include <format>
#include <cassert>
#include "../Events/Event.h"
#include "PropertyManager.h"
#include "../Data/Any.h"

namespace Sgl
{
	class PropertyMap: public std::unordered_map<PropertyId, Any>
	{
	public:
		void Add(PropertyId id, Any&& object)
		{
			assert(PropertyManager::GetTypeNameBy(id) == object.Type().name());

			emplace(id, std::move(object));
		}

		void Add(PropertyId id, const Any& object)
		{
			assert(PropertyManager::GetTypeNameBy(id) == object.Type().name());

			emplace(id, object);
		}

		template<typename TValue, typename... TArgs>
		void Add(PropertyId id, TArgs&&... args)
		{
			Add(id, Any::New<TValue>(std::forward<TArgs>(args)...));
		}

		template<typename TValue>
		bool TryGetValue(PropertyId id, TValue& value) const
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