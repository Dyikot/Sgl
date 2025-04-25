#pragma once

#include <string_view>

namespace Sgl
{
	class PropertyId final
	{
	public:
		std::string_view name;
		const std::type_info& type;
	public:
		constexpr PropertyId(std::string_view name,const std::type_info& type):
			name(name), type(type)
		{}

		template<typename T>
		static constexpr PropertyId New(std::string_view name)
		{ 
			return PropertyId(name, typeid(T));
		}

		friend constexpr bool operator==(const PropertyId& left, const PropertyId& right)
		{
			return left.name == right.name;
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
			return hash<std::string_view>()(id.name);
		}
	};
}