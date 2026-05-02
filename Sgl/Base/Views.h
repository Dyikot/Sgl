#pragma once

#include <ranges>

namespace Sgl::Views
{
	template<typename T>
	constexpr auto Cast()
	{
		return std::views::transform([](auto&& item)
		{
			using TItem = std::decay_t<decltype(item)>;

			if constexpr(std::ranges::range<TItem>)
			{
				return T(std::ranges::begin(item), std::ranges::end(item));
			}
			else
			{
				return static_cast<T>(item);
			}
		});
	}
}