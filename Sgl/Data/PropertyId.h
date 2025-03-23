#pragma once

#include <string_view>

namespace Sgl
{
	class PropertyId final
	{
	public:
		std::string_view Name;
		const std::type_info& Type;
	public:
		constexpr PropertyId(std::string_view name,const std::type_info& type)
			: Name(name), Type(type) {}

		template<typename T>
		static constexpr PropertyId New(std::string_view name)
		{ 
			return PropertyId(name, typeid(T));
		}

		friend constexpr bool operator==(const PropertyId& left, const PropertyId& right)
		{
			return left.Name == right.Name;
		}

		friend constexpr bool operator!=(const PropertyId& left, const PropertyId& right)
		{
			return !operator==(left, right);
		}
	};
}

namespace std
{
	template<>
	struct hash<Sgl::PropertyId>
	{
		size_t operator()(const Sgl::PropertyId& id) const
		{
			return hash<std::string_view>()(id.Name);
		}
	};
}