#pragma once

#include <type_traits>

namespace Sgl
{
	#define CreateObjectId(typeName) \
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

	CreateObjectId(PropertyId);
	CreateObjectId(EventId);	
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