#pragma once

#include <string>
#include "../Visual/Color.h"

namespace Sgl
{
	enum class TextDecoration
	{
		None, Underline, Strikethrough
	};

	struct FontFamily
	{
		static const FontFamily& Get(std::string_view name);

		std::string Name;
		std::string Path;
	};

	enum class FontWeight
	{
		Normal, Bold
	};

	enum class FontStyle
	{
		Normal, Italic
	};
}