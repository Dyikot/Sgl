#include "Font.h"
#include <unordered_map>

namespace Sgl
{
	const std::unordered_map<std::string_view, FontFamily> DefaultFontFamilies =
	{
		{ "Segoe UI", FontFamily("Segoe UI", "") }
	};

	FontFamily FontFamily::Get(std::string_view name)
	{
		return DefaultFontFamilies.at(name);
	}

	FontFamily FontFamily::Default()
	{
		return DefaultFontFamilies.at("Segoe UI");
	}
}