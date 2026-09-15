#pragma once

#include <concepts>

namespace Sgl
{
	//! @brief Base class for all property types
	class PropertyBase
	{
	public:
		friend bool operator==(const PropertyBase& left, const PropertyBase& right)
		{
			return &left == &right;
		}
	};

	//! @brief Concept that defines the requirements for a property type
	template<typename T, typename TOwner = T::Owner, typename TValue = T::Value>
	concept CProperty = requires(T property, TOwner& owner, TValue value)
	{
		{ property.InvokeSetter(owner, value) } -> std::same_as<void>;
		{ property.InvokeGetter(owner) } -> std::same_as<TValue>;
	};
}