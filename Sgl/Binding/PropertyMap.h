#pragma once

#include <format>
#include "../Events/Event.h"
#include "PropertyManagers.h"

namespace Sgl
{
	class PropertyMap: protected std::unordered_map<PropertyId, Any>
	{
	public:
		using Base = std::unordered_map<PropertyId, Any>;
	public:
		Base::iterator begin() { return Base::begin(); }
		Base::iterator end() { return Base::end(); }
		Base::const_iterator begin() const { return Base::begin(); }
		Base::const_iterator end() const { return Base::end(); }
		Any& At(PropertyId id) { return Base::at(id); }
		const Any& At(PropertyId id) const { return Base::at(id); }
		bool Empty() const { return Base::empty(); }
		size_t Count() const { return Base::size(); }
		bool Contains(PropertyId id) const { return Base::contains(id); }
		void Remove(PropertyId id) { Base::erase(id); }

		void Add(PropertyId id, Any&& object)
		{
			if(!IsTypeCorrect(id, object))
			{
				throw std::invalid_argument(
					std::format("Property type <{}> is not equal object type <{}>\n",
								PropertyManager::GetTypeNameBy(id),
								object.Type().name()));
			}

			Base::emplace(id, std::move(object));
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

			Base::emplace(id, object);
		}

		template<typename TValue, typename... TArgs>
		void Add(PropertyId id, TArgs&&... args)
		{
			Add(id, Any::New<TValue>(std::forward<TArgs>(args)...));
		}

		template<typename TValue>
		bool TryGetValue(PropertyId id, TValue& value) const
		{
			if(Contains(id))
			{
				value = At(id).As<TValue>();
				return true;
			}

			return false;
		}

		Any& operator[](const PropertyId& id) { return Base::operator[](id); }
	private:
		bool IsTypeCorrect(PropertyId id, const Any& object) const
		{
			return PropertyManager::GetTypeNameBy(id) == object.Type().name();
		}
	};	
}