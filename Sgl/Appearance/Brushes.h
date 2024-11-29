#pragma once
#include <SDL/SDL_render.h>
#include <variant>

namespace Sgl
{
	class ColorBrush
	{
	public:
		uint8_t R, G, B, A;
	protected:
		static constexpr uint8_t Opaque = 0xff;
		static constexpr uint8_t Transparent = 0x00;
	public:
		constexpr ColorBrush(uint8_t r, uint8_t g, uint8_t b, uint8_t a):
			R(r), G(g), B(b), A(a)
		{}
		constexpr ColorBrush(uint8_t r, uint8_t g, uint8_t b) :
			ColorBrush(r, g, b, Opaque)
		{}
		constexpr ColorBrush(uint32_t color):
			R(color), G(color >> 8), B(color >> 16), A(color >> 24)
		{}

		bool IsTransparent() const noexcept { return A == Transparent; }

		bool operator==(ColorBrush brush) const 
		{
			return R == brush.R && G == brush.G && B == brush.B;
		}

		bool operator!=(ColorBrush brush) const { return !operator==(brush); }
		operator SDL_Color() const { return SDL_Color{ .r = R, .g = G, .b = B, .a = A }; }
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

	class TextureBrush: public ColorBrush
	{
	public:
		SDL_Texture* RawTexture;
	public:
		TextureBrush(SDL_Texture* texture, ColorBrush brush = Colors::White):
			ColorBrush(brush), RawTexture(texture)
		{}
	};

	using Brush = std::variant<ColorBrush, TextureBrush>;
}