#pragma once

#include "../Appearance/Style/Property.h"
#include "../Events/Event.h"
#include "../Any.h"
#include <format>

namespace Sgl
{
	template<typename TId>
	class SetterMap: public std::unordered_map<TId, Any>
	{
	public:
		using Base = std::unordered_map<TId, Any>;
	public:
		void Add(TId id, Any&& object)
		{
			if(!IsTypeCorrect(id, object))
			{
				throw std::invalid_argument(
					std::format("Property type <{}> is not equal object type <{}>\n",
								UniquePropertyManager<TId>::GetTypeNameOf(id),
								object.Type().name()));
			}

			Base::emplace(id, std::move(object));
		}

		void Add(TId id, const Any& object)
		{
			if(!IsTypeCorrect(id, object))
			{
				throw std::invalid_argument(
					std::format("Property type <{}> is not equal object type <{}>\n",
								UniquePropertyManager<TId>::GetTypeNameOf(id),
								object.Type().name()));
			}

			Base::emplace(id, object);
		}

		SetterMap<TId>& operator=(const SetterMap<TId>& setterMap)
		{
			for(auto& [id, value] : setterMap)
			{
				Base::at(id) = value;
			}

			return *this;
		}
	private:
		bool IsTypeCorrect(TId id, const Any& object) const
		{
			return UniquePropertyManager<TId>::GetTypeNameOf(id) == object.Type().name();
		}
	};
}