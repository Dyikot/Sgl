#pragma once

#include <SDL3/SDL_pixels.h>

namespace Sgl
{
	/// <summary>
	/// Represents a color with red, green, blue, and alpha components.
	/// </summary>
	class Color
	{
	public:
		/// <summary>
		/// Initializes a new instance of the Color structure with individual RGBA values
		/// </summary>
		/// <param name="red">- red component value (0-255)</param>
		/// <param name="green">- green component value (0-255)</param>
		/// <param name="blue">- blue component value (0-255)</param>
		/// <param name="alpha">- alpha component value (0-255)</param>
		constexpr Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha):
			Red(red), Green(green), Blue(blue), Alpha(alpha)
		{}

		/// <summary>
		/// Initializes a new instance of the Color structure with RGB values and full opacity
		/// </summary>
		/// <param name="red">- red component value (0-255)</param>
		/// <param name="green">- green component value (0-255)</param>
		/// <param name="blue">- blue component value (0-255)</param>
		constexpr Color(uint8_t red, uint8_t green, uint8_t blue):
			Color(red, green, blue, 0xff)
		{}

		/// <summary>
		/// Initializes a new instance of the Color structure from a 32-bit RGBA value
		/// </summary>
		/// <param name="rgba">- 32-bit value where bits 31-24 (Red), 23-16 (Green), 15-8 (Blue), 7-0 (Alpha)</param>
		constexpr explicit Color(uint32_t rgba):
			Red(rgba >> 24), Green(rgba >> 16), Blue(rgba >> 8), Alpha(rgba)
		{}

		/// <summary>
		/// Red component value (0-255)
		/// </summary>
		uint8_t Red;

		/// <summary>
		/// Green component value (0-255)
		/// </summary>
		uint8_t Green;

		/// <summary>
		/// Blue component value (0-255)
		/// </summary>
		uint8_t Blue;

		/// <summary>
		/// Alpha (transparency) component value (0-255, where 0 is fully transparent and 255 is fully opaque)
		/// </summary>
		uint8_t Alpha;

		/// <summary>
		/// Determines whether this color is fully transparent
		/// </summary>
		/// <returns>True if Alpha is 0, false otherwise</returns>
		constexpr bool IsTransparent() const noexcept
		{
			return Alpha == 0x00;
		}

		/// <summary>
		/// Converts the color to a 32-bit unsigned integer in RGBA format,
		/// where each component occupies one byte in big-endian order: 0xRRGGBBAA.
		/// </summary>
		/// <returns>A uint32_t value representing the color in RGBA layout.</returns>
		constexpr uint32_t ToRgba() const noexcept
		{
			return (Red << 24) | (Green << 16) | (Blue << 8) | Alpha;
		}

		/// <summary>
		/// Converts the color to a 32-bit unsigned integer in ARGB format,
		/// where each component occupies one byte in big-endian order: 0xAARRGGBB.
		/// Commonly used in graphics APIs such as DirectX and Windows GDI.
		/// </summary>
		constexpr uint32_t ToArgb() const noexcept
		{
			return (Alpha << 24) | (Red << 16) | (Green << 8) | Blue;
		}

		/// <summary>
		/// Compares this color with another color for equality (RGB components only)
		/// </summary>
		/// <param name="color">- the color to compare with</param>
		/// <returns>True if RGB components are equal, false otherwise</returns>
		constexpr bool operator==(Color color) const
		{
			return Red == color.Red && Green == color.Green && Blue == color.Blue;
		}

		constexpr operator SDL_Color() const
		{
			return SDL_Color(Red, Green, Blue, Alpha);
		}

		constexpr operator SDL_FColor() const
		{
			return SDL_FColor(
				static_cast<float>(Red) / 255.f, 
				static_cast<float>(Green) / 255.f, 
				static_cast<float>(Blue) / 255.f, 
				static_cast<float>(Alpha) / 255.f
			);
		}
	};

	namespace Colors
	{
		inline constexpr Color AliceBlue { 0xf0f8ffff };
		inline constexpr Color AntiqueWhite { 0xfaebd7ff };
		inline constexpr Color Aqua { 0x00ffffff };
		inline constexpr Color Aquamarine { 0x7fffd4ff };
		inline constexpr Color Azure { 0xf0ffffff };
		inline constexpr Color Beige { 0xf5f5dcff };
		inline constexpr Color Bisque { 0xffe4c4ff };
		inline constexpr Color Black { 0x000000ff };
		inline constexpr Color BlanchedAlmond { 0xffebcdff };
		inline constexpr Color Blue { 0x1e90ffff };
		inline constexpr Color BlueViolet { 0x8a2be2ff };
		inline constexpr Color Brown { 0xa52a2aff };
		inline constexpr Color BurlyWood { 0xdeb887ff };
		inline constexpr Color CadetBlue { 0x5f9ea0ff };
		inline constexpr Color Chartreuse { 0x7fff00ff };
		inline constexpr Color Chocolate { 0xd2691eff };
		inline constexpr Color Coral { 0xff7f50ff };
		inline constexpr Color CornflowerBlue { 0x6495edff };
		inline constexpr Color Cornsilk { 0xfff8dcff };
		inline constexpr Color Crimson { 0xdc143cff };
		inline constexpr Color Cyan { 0x00ced1ff };
		inline constexpr Color DarkBlue { 0x00008bff };
		inline constexpr Color DarkCyan { 0x008b8bff };
		inline constexpr Color DarkGoldenrod { 0xb8860bff };
		inline constexpr Color DarkGray { 0xa9a9a9ff };
		inline constexpr Color DarkGreen { 0x006400ff };
		inline constexpr Color DarkKhaki { 0xbdb76bff };
		inline constexpr Color DarkMagenta { 0x8b008bff };
		inline constexpr Color DarkOliveGreen { 0x556b2fff };
		inline constexpr Color DarkOrange { 0xff8c00ff };
		inline constexpr Color DarkOrchid { 0x9932ccff };
		inline constexpr Color DarkRed { 0x8b0000ff };
		inline constexpr Color DarkSalmon { 0xe9967aff };
		inline constexpr Color DarkSeaGreen { 0x8fbc8bff };
		inline constexpr Color DarkSlateBlue { 0x483d8bff };
		inline constexpr Color DarkSlateGray { 0x2f4f4fff };
		inline constexpr Color DarkTurquoise { 0x00ced1ff };
		inline constexpr Color DarkViolet { 0x9400d3ff };
		inline constexpr Color DeepPink { 0xff1493ff };
		inline constexpr Color DeepSkyBlue { 0x00bfffff };
		inline constexpr Color DimGray { 0x696969ff };
		inline constexpr Color DodgerBlue { 0x1e90ffff };
		inline constexpr Color Firebrick { 0xb22222ff };
		inline constexpr Color FloralWhite { 0xfffaf0ff };
		inline constexpr Color ForestGreen { 0x228b22ff };
		inline constexpr Color Fuchsia { 0xff00ffff };
		inline constexpr Color Gainsboro { 0xdcdcdcff };
		inline constexpr Color GhostWhite { 0xf8f8ffff };
		inline constexpr Color Gold { 0xffd700ff };
		inline constexpr Color Goldenrod { 0xdaa520ff };
		inline constexpr Color Gray { 0x808080ff };
		inline constexpr Color Green { 0x008000ff };
		inline constexpr Color GreenYellow { 0xadff2fff };
		inline constexpr Color Grey { 0x6e6e6eff };
		inline constexpr Color Honeydew { 0xf0fff0ff };
		inline constexpr Color HotPink { 0xff69b4ff };
		inline constexpr Color IndianRed { 0xcd5c5cff };
		inline constexpr Color Indigo { 0x4b0082ff };
		inline constexpr Color Ivory { 0xfffff0ff };
		inline constexpr Color Khaki { 0xf0e68cff };
		inline constexpr Color Lavender { 0xe6e6faff };
		inline constexpr Color LavenderBlush { 0xfff0f5ff };
		inline constexpr Color LawnGreen { 0x7cfc00ff };
		inline constexpr Color LemonChiffon { 0xfffacdff };
		inline constexpr Color LightBlue { 0xadd8e6ff };
		inline constexpr Color LightCoral { 0xf08080ff };
		inline constexpr Color LightCyan { 0xe0ffffff };
		inline constexpr Color LightGoldenrodYellow { 0xfafad2ff };
		inline constexpr Color LightGray { 0xd3d3d3ff };
		inline constexpr Color LightGreen { 0x00af00ff };
		inline constexpr Color LightPink { 0xffb6c1ff };
		inline constexpr Color LightSalmon { 0xffa07aff };
		inline constexpr Color LightSeaGreen { 0x20b2aaff };
		inline constexpr Color LightSkyBlue { 0x87cefaff };
		inline constexpr Color LightSlateGray { 0x778899ff };
		inline constexpr Color LightSteelBlue { 0xb0c4deff };
		inline constexpr Color LightYellow { 0xffffe0ff };
		inline constexpr Color Lime { 0x00ff00ff };
		inline constexpr Color LimeGreen { 0x32cd32ff };
		inline constexpr Color Linen { 0xfaf0e6ff };
		inline constexpr Color Magenta { 0xff00ffff };
		inline constexpr Color Maroon { 0x800000ff };
		inline constexpr Color MediumAquamarine { 0x66cdaaff };
		inline constexpr Color MediumBlue { 0x0000cdff };
		inline constexpr Color MediumOrchid { 0xba55d3ff };
		inline constexpr Color MediumPurple { 0x9370dbff };
		inline constexpr Color MediumSeaGreen { 0x3cb371ff };
		inline constexpr Color MediumSlateBlue { 0x7b68eeff };
		inline constexpr Color MediumSpringGreen { 0x00fa9aff };
		inline constexpr Color MediumTurquoise { 0x48d1ccff };
		inline constexpr Color MediumVioletRed { 0xc71585ff };
		inline constexpr Color MidnightBlue { 0x191970ff };
		inline constexpr Color MintCream { 0xf5fffaff };
		inline constexpr Color MistyRose { 0xffe4e1ff };
		inline constexpr Color Moccasin { 0xffe4b5ff };
		inline constexpr Color NavajoWhite { 0xffdeadff };
		inline constexpr Color Navy { 0x000080ff };
		inline constexpr Color OldLace { 0xfdf5e6ff };
		inline constexpr Color Olive { 0x808000ff };
		inline constexpr Color OliveDrab { 0x6b8e23ff };
		inline constexpr Color Orange { 0xff8c00ff };
		inline constexpr Color OrangeRed { 0xff4500ff };
		inline constexpr Color Orchid { 0xda70d6ff };
		inline constexpr Color PaleGoldenrod { 0xeee8aaff };
		inline constexpr Color PaleGreen { 0x98fb98ff };
		inline constexpr Color PaleTurquoise { 0xafeeeeff };
		inline constexpr Color PaleVioletRed { 0xdb7093ff };
		inline constexpr Color PapayaWhip { 0xffefd5ff };
		inline constexpr Color PeachPuff { 0xffdab9ff };
		inline constexpr Color Peru { 0xcd853fff };
		inline constexpr Color Pink { 0xff69b4ff };
		inline constexpr Color Plum { 0xdda0ddff };
		inline constexpr Color PowderBlue { 0xb0e0e6ff };
		inline constexpr Color Purple { 0x800080ff };
		inline constexpr Color RebeccaPurple { 0x663399ff };
		inline constexpr Color Red { 0xd22b2bff };
		inline constexpr Color RosyBrown { 0xbc8f8fff };
		inline constexpr Color RoyalBlue { 0x4169e1ff };
		inline constexpr Color SaddleBrown { 0x8b4513ff };
		inline constexpr Color Salmon { 0xfa8072ff };
		inline constexpr Color SandyBrown { 0xf4a460ff };
		inline constexpr Color SeaGreen { 0x2e8b57ff };
		inline constexpr Color SeaShell { 0xfff5eeff };
		inline constexpr Color Sienna { 0xa0522dff };
		inline constexpr Color Silver { 0xc0c0c0ff };
		inline constexpr Color SkyBlue { 0x87ceebff };
		inline constexpr Color SlateBlue { 0x6a5acdff };
		inline constexpr Color SlateGray { 0x708090ff };
		inline constexpr Color Snow { 0xfffafaff };
		inline constexpr Color SpringGreen { 0x00ff7fff };
		inline constexpr Color SteelBlue { 0x4682b4ff };
		inline constexpr Color Tan { 0xd2b48cff };
		inline constexpr Color Teal { 0x008080ff };
		inline constexpr Color Thistle { 0xd8bfd8ff };
		inline constexpr Color Tomato { 0xff6347ff };
		inline constexpr Color Transparent { 0x00000000 };
		inline constexpr Color Turquoise { 0x40e0d0ff };
		inline constexpr Color Violet { 0xee82eeff };
		inline constexpr Color Wheat { 0xf5deb3ff };
		inline constexpr Color White { 0xffffffff };
		inline constexpr Color WhiteSmoke { 0xe6e6e6ff };
		inline constexpr Color Yellow { 0xdcdc00ff };
		inline constexpr Color YellowGreen { 0x9acd32ff };
	}
}