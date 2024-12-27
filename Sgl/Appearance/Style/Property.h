#pragma once

#include <unordered_set>
#include <unordered_map>
#include <string>
#include <typeinfo>
#include <stdexcept>

namespace Sgl
{
	#define CreateSizeType(typeName) \
	class typeName \
	{ \
	private:\
		size_t _value;\
	public:\
		constexpr typeName(size_t value):\
			_value(value)\
		{}\
		constexpr operator size_t() const { return _value; }\
	}\

	CreateSizeType(PropertyId);
	CreateSizeType(EventId);

	template<typename TId>
	class UniquePropertyManager
	{
	private:
		static inline std::unordered_map<TId, std::string> _propertTypeNamesMap;
		static inline std::unordered_set<std::string> _propertiesNames;
	public:
		template<typename T>
		static const TId Register(std::string&& name)
		{
			auto [_, isInserted] = _propertiesNames.insert(std::move(name));
			
			if(isInserted)
			{
				TId id = _propertiesNames.size() - 1;
				_propertTypeNamesMap.emplace(id, typeid(T).name());
				return id;
			}
			else
			{
				throw std::invalid_argument("Property with this name already exist\n");
			}
		}

		static std::string_view GetTypeNameOf(TId id)
		{
			return _propertTypeNamesMap.at(id);
		}
	};

	using PropertyManager = UniquePropertyManager<PropertyId>;
	using EventManager = UniquePropertyManager<EventId>;
}

template<>
struct std::hash<Sgl::PropertyId>
{
	size_t operator()(const Sgl::PropertyId& id) const
	{
		return std::hash<size_t>()(id);
	}
};

template<>
struct std::hash<Sgl::EventId>
{
	size_t operator()(const Sgl::EventId& id) const
	{
		return std::hash<size_t>()(id);
	}
};