#pragma once

#include <format>
#include "../Events/Event.h"
#include "PropertyManagers.h"

namespace Sgl
{
	class PropertyMap: public std::unordered_map<PropertyId, Any>
	{
	public:
		void Add(PropertyId id, Any&& object)
		{
			if(!IsTypeCorrect(id, object))
			{
				throw std::invalid_argument(
					std::format("Property type <{}> is not equal object type <{}>\n",
								PropertyManager::GetTypeNameBy(id),
								object.Type().name()));
			}

			emplace(id, std::move(object));
		}

		void Add(PropertyId id, const Any& object)
		{
			if(!IsTypeCorrect(id, object))
			{
				throw std::invalid_argument(
					std::format("Property type <{}> is not equal object type <{}>\n",
								PropertyManager::GetTypeNameBy(id),
								object.Type().name()));
			}

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
	private:
		bool IsTypeCorrect(PropertyId id, const Any& object) const
		{
			return PropertyManager::GetTypeNameBy(id) == object.Type().name();
		}
	};	
}