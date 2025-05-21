#pragma once

#include <cstdint>
#include "../Graphic/Color.h"
#include <string>

namespace Sgl
{
	struct Thickness
	{
	public:
		uint32_t Top;
		uint32_t Right;
		uint32_t Bottom;
		uint32_t Left;
	public:
		constexpr Thickness(uint32_t top, uint32_t right, uint32_t bottom, uint32_t left) noexcept:
			Top(top), Right(right), Bottom(bottom), Left(left)
		{}

		constexpr Thickness(uint32_t value = 0) noexcept:
			Thickness(value, value, value, value) 
		{}

		friend constexpr bool operator==(const Thickness& left, const Thickness& right)
		{
			return left.Top == right.Top &&
				   left.Right == right.Right &&
				   left.Bottom == right.Bottom &&
				   left.Left == right.Left;
		}
	};

	class FontFamily
	{		
	private:
		std::string _name;
	public:
		static FontFamily Get(std::string_view name);
		static FontFamily Default();

		FontFamily(std::string name, std::string path):
			_name(std::move(name))
		{}

		std::string_view GetName() const
		{
			return _name;
		}
	};

	enum class FontWeight
	{
		Normal, Bold
	};

	enum class FontStyle
	{
		Normal, Italic
	};

	struct Font
	{
		int Size;
		FontFamily Family;
		FontStyle Style;
		FontWeight Weight;
		Color Color;
	};

	struct Border
	{
		Thickness Thickness;
		Color Color;
	};

	enum class TextDecorations
	{
		None, Underline, Strikethrough
	};

	enum class HorizontalAlignment
	{
		Center, Left, Right, Stretch
	};

	enum class VerticalAlignment
	{
		Bottom, Center, Top, Stretch
	};

	enum class Visibility
	{
		Visible, Hidden, Collapsed
	};
}