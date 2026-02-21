#pragma once

#include <concepts>
#include <string>

namespace Sgl
{
	template<typename T, typename TFrom = T::From, typename TTo = T::To>
	concept CValueConverter = requires(T converter, TFrom from, TTo to)
	{
		{ converter.Convert(from) } -> std::same_as<TTo>;
		{ converter.ConvertBack(to) } -> std::same_as<TFrom>;
	};
}

namespace Sgl::Converters
{
	struct IntToStringConverter
	{
		using From = int;
		using To = std::string;

		std::string Convert(int value) const;
		int ConvertBack(const std::string& value) const;
	};

	inline constexpr IntToStringConverter IntToString;
}