#pragma once

#include <unordered_set>
#include <unordered_map>
#include <string>
#include <typeinfo>
#include <stdexcept>
#include <functional>

namespace Sgl
{
	using PropertyId = size_t;

	template<typename T>
	class Property;

	class IProperty
	{
	public:
		virtual ~IProperty() = default;

		virtual const std::type_info& Type() const = 0;
		virtual IProperty* Copy() const = 0;

		template<typename T>
		const T& Get() const
		{
			return static_cast<const Property<T>*>(this)->Value;
		}

		template<typename T>
		void Set(const T& value)
		{
			static_cast<Property<T>*>(this)->Value = value;
		}
	};

	template<typename T>
	class Property: public IProperty
	{
	public:
		T Value;
	public:
		Property():
			Value()
		{}
		Property(auto&&... args):
			Value(args...)
		{}
		Property(const T& value):
			Value(value)
		{}

		IProperty* Copy() const override
		{
			return new Property<T>(*this);
		}

		const std::type_info& Type() const override
		{
			return typeid(Value);
		}
	};

	class PropertyManager
	{
	private:
		static inline std::unordered_map<PropertyId, std::string> _idToTypeNameMap = {};
		static inline std::unordered_set<std::string> _propertiesNames = {};
	public:
		template<typename Type>
		static const PropertyId Register(std::string&& name)
		{
			auto [_, isInserted] = _propertiesNames.insert(std::move(name));
			
			if(isInserted)
			{
				PropertyId id = _propertiesNames.size() - 1;
				_idToTypeNameMap.emplace(id, typeid(Type).name());
				return id;
			}
			else
			{
				throw std::invalid_argument("Property with this name already exist\n");
			}
		}
		static std::string_view GetTypeNameBy(const PropertyId id);
	};	
}