#pragma once
#include <SDL/SDL_render.h>
#include <variant>

namespace Sgl
{
	struct ColorBrush
	{
		uint8_t R, G, B, A;

		constexpr ColorBrush(uint8_t r, uint8_t g, uint8_t b, uint8_t a):
			R(r), G(g), B(b), A(a)
		{}
		constexpr ColorBrush(uint8_t r, uint8_t g, uint8_t b) :
			ColorBrush(r, g, b, 0xff)
		{}
		constexpr ColorBrush(uint32_t color):
			R(color), G(color >> 8), B(color >> 16), A(color >> 24)
		{}
	};	

	namespace Colors
	{
		inline constexpr ColorBrush Transparent = 0x00000000;
		inline constexpr ColorBrush Black = 0x141414ff;
		inline constexpr ColorBrush Green = 0x008000ff;
		inline constexpr ColorBrush Blue = 0x1e90ffff;
		inline constexpr ColorBrush Red = 0xd22b2bff;
		inline constexpr ColorBrush Cyan = 0x00ced1ff;
		inline constexpr ColorBrush Yellow = 0xdcdc00ff;
		inline constexpr ColorBrush Pink = 0xff69b4ff;
		inline constexpr ColorBrush Orange = 0xff8c00ff;
		inline constexpr ColorBrush White = 0xffffffff;
		inline constexpr ColorBrush Grey = 0x6e6e6eff;
		inline constexpr ColorBrush Whitesmoke = 0xe6e6e6ff;
		inline constexpr ColorBrush Lightgreen = 0x00af00ff;
		inline constexpr ColorBrush Darkgrey = 0x464646ff;
	}

	struct TextureBrush: ColorBrush
	{
	public:
		SDL_Texture* Source;
	public:
		TextureBrush(SDL_Texture* texture, ColorBrush brush = Colors::White):
			ColorBrush(brush), Source(texture)
		{}
	};

	using Brush = std::variant<ColorBrush, TextureBrush>;
}