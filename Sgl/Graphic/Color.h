#pragma once
#include <SDL/SDL_render.h>

namespace Sgl
{
	class Color
	{
	public:
		uint8_t R, G, B, A;
	private:
		static constexpr uint8_t Opaque = 0xff;
		static constexpr uint8_t Transparent = 0x00;
	public:
		constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a):
			R(r), G(g), B(b), A(a)
		{}
		constexpr Color(uint8_t r, uint8_t g, uint8_t b) :
			Color(r, g, b, Opaque)
		{}
		constexpr explicit Color(uint32_t rgba):
			R(rgba >> 24), G(rgba >> 16), B(rgba >> 8), A(rgba)
		{}

		constexpr bool IsTransparent() const noexcept { return A == Transparent; }

		constexpr bool operator==(Color color) const 
		{
			return R == color.R && G == color.G && B == color.B;
		}

		constexpr bool operator!=(Color color) const { return !operator==(color); }
		constexpr operator SDL_Color() const { return SDL_Color{ R, G, B, A }; }
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