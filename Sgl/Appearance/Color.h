#pragma once
#include <SDL/SDL_render.h>

namespace Sgl
{
	class Color
	{
	public:
		uint8_t R, G, B, A;
	protected:
		static constexpr uint8_t Opaque = 0xff;
		static constexpr uint8_t Transparent = 0x00;
	public:
		constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a):
			R(r), G(g), B(b), A(a)
		{}
		constexpr Color(uint8_t r, uint8_t g, uint8_t b) :
			Color(r, g, b, Opaque)
		{}
		constexpr Color(uint32_t color):
			R(color), G(color >> 8), B(color >> 16), A(color >> 24)
		{}

		constexpr bool IsTransparent() const noexcept { return A == Transparent; }

		constexpr bool operator==(Color color) const 
		{
			return R == color.R && G == color.G && B == color.B;
		}
		constexpr bool operator!=(Color color) const { return !operator==(color); }
		constexpr operator SDL_Color() const { return { R, G, B, A }; }
	};

	namespace Colors
	{
		inline constexpr Color Transparent = 0x00000000;
		inline constexpr Color Black = 0x141414ff;
		inline constexpr Color Green = 0x008000ff;
		inline constexpr Color Blue = 0x1e90ffff;
		inline constexpr Color Red = 0xd22b2bff;
		inline constexpr Color Cyan = 0x00ced1ff;
		inline constexpr Color Yellow = 0xdcdc00ff;
		inline constexpr Color Pink = 0xff69b4ff;
		inline constexpr Color Orange = 0xff8c00ff;
		inline constexpr Color White = 0xffffffff;
		inline constexpr Color Grey = 0x6e6e6eff;
		inline constexpr Color Whitesmoke = 0xe6e6e6ff;
		inline constexpr Color Lightgreen = 0x00af00ff;
		inline constexpr Color Darkgrey = 0x464646ff;
	}
}