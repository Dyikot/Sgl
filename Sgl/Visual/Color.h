#pragma once

#include <SDL/SDL_render.h>

namespace Sgl
{
	class Color
	{
	public:
		uint8_t Red;
		uint8_t Green;
		uint8_t Blue;
		uint8_t Alpha;
	private:
		static constexpr uint8_t _opaque = 0xff;
		static constexpr uint8_t _transparent = 0x00;
	public:
		constexpr Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha):
			Red(red), Green(green), Blue(blue), Alpha(alpha)
		{}

		constexpr Color(uint8_t red, uint8_t green, uint8_t blue):
			Color(red, green, blue, _opaque)
		{}

		constexpr explicit Color(uint32_t rgba):
			Red(rgba >> 24), Green(rgba >> 16), Blue(rgba >> 8), Alpha(rgba)
		{}

		constexpr bool IsTransparent() const noexcept 
		{
			return Alpha == _transparent;
		}

		constexpr bool operator==(Color color) const 
		{
			return Red == color.Red && Green == color.Green && Blue == color.Blue;
		}

		constexpr operator SDL_Color() const 
		{ 
			return SDL_Color { Red, Green, Blue, Alpha };
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