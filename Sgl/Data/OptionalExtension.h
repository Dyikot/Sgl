#pragma once

#include <optional>

namespace Sgl
{
	template<typename T>
	inline const T& operator|(const std::optional<T>& left, T&& value)
	{
		return left.value_or(std::forward<T>(value));
	}

	template<typename T>
	inline const std::optional<T>& operator|(const std::optional<T>& left,
											 const std::optional<T>& right)
	{
		return left.has_value() ? left : right;
	}

	template<typename T>
	inline const T& operator|=(std::optional<T>& left, T&& value)
	{
		if(!left.has_value())
		{
			left = std::forward<T>(value);
		}

		return left.value();
	}
}