#pragma once
#include <SDL/SDL_render.h>

namespace Sgl
{
	class Color
	{
	public:
		uint8_t r, g, b, a;
	private:
		static constexpr uint8_t _opaque = 0xff;
		static constexpr uint8_t _transparent = 0x00;
	public:
		constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a):
			r(r), g(g), b(b), a(a)
		{}

		constexpr Color(uint8_t r, uint8_t g, uint8_t b):
			Color(r, g, b, _opaque)
		{}

		constexpr explicit Color(uint32_t rgba):
			r(rgba >> 24), g(rgba >> 16), b(rgba >> 8), a(rgba)
		{}

		constexpr bool IsTransparent() const noexcept 
		{
			return a == _transparent;
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
		inline constexpr Color Transparent = Color(0x00000000);
		inline constexpr Color Black = Color(0x000000ff);
		inline constexpr Color Green = Color(0x008000ff);
		inline constexpr Color Blue = Color(0x1e90ffff);
		inline constexpr Color Red = Color(0xd22b2bff);
		inline constexpr Color Cyan = Color(0x00ced1ff);
		inline constexpr Color Yellow = Color(0xdcdc00ff);
		inline constexpr Color Pink = Color(0xff69b4ff);
		inline constexpr Color Orange = Color(0xff8c00ff);
		inline constexpr Color White = Color(0xffffffff);
		inline constexpr Color Grey = Color(0x6e6e6eff);
		inline constexpr Color Whitesmoke = Color(0xe6e6e6ff);
		inline constexpr Color Lightgreen = Color(0x00af00ff);
		inline constexpr Color Darkgrey = Color(0x464646ff);
	}
}