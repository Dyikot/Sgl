#include "Font.h"
#include <unordered_map>

namespace Sgl
{
	static const std::unordered_map<std::string_view, FontFamily> FontFamilies =
	{
		{ "SegoeUI", { "Segoe UI", "" } },
	};

	const FontFamily& FontFamily::Get(std::string_view name)
	{
		return FontFamilies.at(name);
	}
}