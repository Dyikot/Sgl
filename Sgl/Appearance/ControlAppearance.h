#pragma once

#include <cstdint>
#include <filesystem>

namespace Sgl
{
	struct Thikness
	{
		Thikness(uint32_t top, uint32_t right, uint32_t bottom, uint32_t left):
			Top(top), Right(right), Bottom(bottom), Left(left)
		{}

		Thikness(uint32_t lenght = 0):
			Thikness(lenght, lenght, lenght, lenght) 
		{}

		uint32_t Top;
		uint32_t Right;
		uint32_t Bottom;
		uint32_t Left;
	};

	class FontFamily
	{
	public:
		static constexpr std::string_view DefaultFamilyName = "Segoe UI";

		std::string Name;
		std::filesystem::path Path;
	public:
		FontFamily(std::string_view familyName, const std::filesystem::path& path):
			Name(familyName), Path(path) 
		{}

		FontFamily(std::string_view familyName = DefaultFamilyName):
			FontFamily(familyName, GetPathBy(familyName)) 
		{}
	private:
		std::filesystem::path GetPathBy(std::string_view familyName)
		{
			return std::format("Resources/Fonts/{}.ttf", familyName);
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
}