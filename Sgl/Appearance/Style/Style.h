#pragma once

#include "Property.h"
#include "../../Events/Event.h"
#include "../../Any.h"

namespace Sgl
{
	class Style
	{		
	private:
		std::unordered_map<PropertyId, Any> _propertyMap;
	public:
		Style() noexcept;
		Style(const Style& style) noexcept;
		
		void Add(PropertyId id, Any&& object)
		{
			if(!IsTypeCorrect(id, object))
			{
				throw std::invalid_argument("Id type and property type is equal!");
			}

			_propertyMap.emplace(id, std::move(object));
		}

		void Add(PropertyId id, const Any& object)
		{
			if(!IsTypeCorrect(id, object))
			{
				throw std::invalid_argument("Id type and property type is equal!");
			}

			_propertyMap.emplace(id, object);
		}

		void Add(const Style& style);
		void Remove(PropertyId id) noexcept;
		bool Contains(PropertyId id) const noexcept;
		void Clear() noexcept;
		size_t Count() const noexcept;

		template<typename T>
		void TryInit(PropertyId id, T& value) const
		{
			if(_propertyMap.contains(id))
			{
				value = _propertyMap.at(id).As<T>();
			}
		}

		template<typename T>
		void TryInit(PropertyId id, Event<T>& event) const
		{
			if(_propertyMap.contains(id))
			{
				event += _propertyMap.at(id).As<T>();
			}
		}

		Any& operator[](PropertyId id);
	private:
		bool IsTypeCorrect(PropertyId id, const Any& object) const;
	};
}