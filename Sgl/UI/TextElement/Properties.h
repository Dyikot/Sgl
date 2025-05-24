#pragma once

#include <string>
#include "../../Graphic/Color.h"

namespace Sgl
{
	enum class TextDecoration
	{
		None, Underline, Strikethrough
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
		int Size = 14;
		FontFamily Family = FontFamily::Default();
		FontStyle Style = FontStyle::Normal;
		FontWeight Weight = FontWeight::Normal;
		Color Color = Colors::Black;
	};
}