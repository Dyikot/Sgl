#pragma once

#include <cstdint>
#include <filesystem>

namespace Sgl
{
	struct Thikness
	{
	public:
		uint32_t Top;
		uint32_t Right;
		uint32_t Bottom;
		uint32_t Left;
	public:
		constexpr Thikness(uint32_t top, uint32_t right, uint32_t bottom, uint32_t left) noexcept:
			Top(top), Right(right), Bottom(bottom), Left(left)
		{}

		constexpr Thikness(uint32_t lenght = 0) noexcept:
			Thikness(lenght, lenght, lenght, lenght) 
		{}

		friend constexpr bool operator==(const Thikness& left, const Thikness& right)
		{
			return left.Top == right.Top &&
				   left.Right == right.Right &&
				   left.Bottom == right.Bottom &&
				   left.Left == right.Left;
		}
	};

	class FontFamily
	{		
	public:
		std::filesystem::path Path;
		std::string Name;
	public:
		FontFamily():
			FontFamily("Segoe UI")
		{}

		explicit FontFamily(std::string_view name):
			Path(TryGetPathByName(name)), Name(name)
		{}

		FontFamily(const std::filesystem::path& path, std::string_view name):
			Path(path), Name(name)
		{}

		static std::filesystem::path TryGetPathByName(std::string_view name)
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

	enum class TextDecorations
	{
		None, Underline, Strikethrough
	};

	enum class HorizontalAlignment
	{
		Center, Left, Right, Stretch
	};

	enum class VerticalAligment
	{
		Bottom, Center, Top, Stretch
	};

	enum class Visibility
	{
		Visible, Hidden, Collapsed
	};
}