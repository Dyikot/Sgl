#pragma once
#include <SDL/SDL_render.h>

namespace Sgl
{
	class Color
	{
	public:
		uint8_t r, g, b, a;
	private:
		static constexpr uint8_t Opaque = 0xff;
		static constexpr uint8_t Transparent = 0x00;
	public:
		constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a):
			r(r), g(g), b(b), a(a)
		{}

		constexpr Color(uint8_t r, uint8_t g, uint8_t b):
			Color(r, g, b, Opaque)
		{}

		constexpr explicit Color(uint32_t rgba):
			r(rgba >> 24), g(rgba >> 16), b(rgba >> 8), a(rgba)
		{}

		constexpr bool IsTransparent() const noexcept 
		{
			return a == Transparent;
		}

		constexpr bool operator==(Color color) const 
		{
			return r == color.r && g == color.g && b == color.b;
		}

		constexpr operator SDL_Color() const 
		{ 
			return SDL_Color{ r, g, b, a };
		}
	};

	namespace Colors
	{
		inline constexpr Color transparent = Color(0x00000000);
		inline constexpr Color black = Color(0x000000ff);
		inline constexpr Color green = Color(0x008000ff);
		inline constexpr Color blue = Color(0x1e90ffff);
		inline constexpr Color red = Color(0xd22b2bff);
		inline constexpr Color cyan = Color(0x00ced1ff);
		inline constexpr Color yellow = Color(0xdcdc00ff);
		inline constexpr Color pink = Color(0xff69b4ff);
		inline constexpr Color orange = Color(0xff8c00ff);
		inline constexpr Color white = Color(0xffffffff);
		inline constexpr Color grey = Color(0x6e6e6eff);
		inline constexpr Color whitesmoke = Color(0xe6e6e6ff);
		inline constexpr Color lightgreen = Color(0x00af00ff);
		inline constexpr Color darkgrey = Color(0x464646ff);
	}
}