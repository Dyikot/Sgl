#pragma once

#include "Property.h"
#include "../../Events/Event.h"
#include "../../Any.h"
#include <format>
#include <iostream>

namespace Sgl
{
	class Style
	{		
	private:
		std::unordered_map<PropertyId, Any> _propertyMap;
	public:
		Style() noexcept;
		Style(const Style& style) noexcept;
		
		void Add(PropertyId id, Any&& object);
		void Add(PropertyId id, const Any& object);
		void Add(const Style& style);
		void Remove(PropertyId id) noexcept;
		bool Contains(PropertyId id) const noexcept;
		void Clear() noexcept;
		size_t Count() const noexcept;

		template<typename T>
		bool TryInit(PropertyId id, T& value) const
		{
			if(_propertyMap.contains(id))
			{
				value = _propertyMap.at(id).As<T>();
				return true;
			}

			return false;
		}

		template<typename T>
		bool TryInitEvent(PropertyId id, Event<T>& event) const
		{
			if(_propertyMap.contains(id))
			{
				event += _propertyMap.at(id).As<T>();
				return true;
			}

			return false;
		}

		Any& operator[](PropertyId id);
	private:
		bool IsTypeCorrect(PropertyId id, const Any& object) const;
	};
}