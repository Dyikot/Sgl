#pragma once

#include "Property.h"
#include "../../Events/Event.h"

namespace Sgl
{
	class Style
	{		
	private:
		std::unordered_map<PropertyId, IProperty*> _propertyMap;
	public:
		Style() noexcept;
		Style(const Style& style) noexcept;
		~Style() noexcept;

		template<typename T>
		void Add(PropertyId id)
		{
			IProperty* property = new Property<T>();
			if(IsTypeCorrect(id, property))
			{
				throw std::invalid_argument("Id type and property type is equal!");
			}

			_propertyMap.insert(std::make_pair(id, property));
		}

		template<typename T, typename... Args>
		void Add(PropertyId id, Args&&... args)
		{
			IProperty* property = new Property<T>(std::forward<Args>(args)...);
			if(!IsTypeCorrect(id, property))
			{
				throw std::invalid_argument("Id type and property type is equal!");
			}

			_propertyMap.insert(std::make_pair(id, property));
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
				value = _propertyMap.at(id)->Get<T>();
			}
		}

		template<typename T>
		void TryInit(PropertyId id, Event<T>& value) const
		{
			if(_propertyMap.contains(id))
			{
				value += _propertyMap.at(id)->Get<T>();
			}
		}

		template<typename T>
		T GetValue(PropertyId id) const
		{
			if(_propertyMap.contains(id))
			{
				return _propertyMap.at(id)->Get<T>();
			}

			return T{};
		}

		IProperty* const operator[](PropertyId id);
	private:
		bool IsTypeCorrect(PropertyId id, IProperty* property) const;
	};
}