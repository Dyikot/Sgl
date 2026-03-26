#pragma once

#include <concepts>

namespace Sgl
{
	/// <summary>
	/// Base class for all property types.
	/// Provides identity comparison for properties.
	/// </summary>
	class PropertyBase
	{
	public:
		friend bool operator==(const PropertyBase& left, const PropertyBase& right)
		{
			return &left == &right;
		}
	};

	/// <summary>
	/// Concept that defines the requirements for a property type.
	/// A property must support invoking getters and setters on an owner object.
	/// </summary>
	template<typename T, typename TOwner = T::Owner, typename TValue = T::Value>
	concept CProperty = requires(T property, TOwner& owner, TValue value)
	{
		{ property.InvokeSetter(owner, value) } -> std::same_as<void>;
		{ property.InvokeGetter(owner) } -> std::same_as<TValue>;
	};
}