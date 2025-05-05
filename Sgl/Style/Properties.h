#pragma once

#include <cstdint>
#include <filesystem>

namespace Sgl
{
	struct Thickness
	{
	public:
		uint32_t top;
		uint32_t right;
		uint32_t bottom;
		uint32_t left;
	public:
		constexpr Thickness(uint32_t top, uint32_t right, uint32_t bottom, uint32_t left) noexcept:
			top(top), right(right), bottom(bottom), left(left)
		{}

		constexpr Thickness(uint32_t lenght = 0) noexcept:
			Thickness(lenght, lenght, lenght, lenght) 
		{}

		friend constexpr bool operator==(const Thickness& left, const Thickness& right)
		{
			return left.top == right.top &&
				   left.right == right.right &&
				   left.bottom == right.bottom &&
				   left.left == right.left;
		}
	};

	class FontFamily
	{		
	public:
		std::filesystem::path path;
		std::string name;
	public:
		FontFamily():
			FontFamily("Segoe UI")
		{}

		explicit FontFamily(std::string name):
			path(TryGetPathByName(name)), name(std::move(name))
		{}

		FontFamily(std::filesystem::path path, std::string name):
			path(std::move(path)), name(std::move(name))
		{}

		static std::filesystem::path TryGetPathByName(std::string name)
		{
			return std::filesystem::path();
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
		int size;
		FontFamily family;
		FontStyle style;
		FontWeight weight;
		Color color;
	};

	struct Border
	{
		Thickness thickness;
		Color color;
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