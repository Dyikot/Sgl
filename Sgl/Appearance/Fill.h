#pragma once
#include <variant>
#include "Texture.h"

namespace Sgl
{
	using Fill = std::variant<const Color*, const Texture*>;

	template<typename TColorFiller, typename TTextureFiller>
	struct Filler
	{
	public:
		TColorFiller WithColor;
		TTextureFiller WithTexture;
	public:
		constexpr auto FillWith(const Fill& fill) const
		{
			return std::visit([this](auto&& fill)
			{
				using T = std::decay_t<decltype(fill)>;

				if constexpr(std::is_same_v<const Color*, T>)
				{
					return WithColor(*fill);
				}
				else if constexpr(std::is_same_v<const Texture*, T>)
				{
					return WithTexture(*fill);
				}

			}, fill);
		}
	};
}