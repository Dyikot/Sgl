#pragma once

#include "Style.h"
#include <vector>

namespace Sgl
{
	class StyleableElement
	{
	public:
		std::vector<Style> Styles;
		std::vector<std::string> Classes;
	public:
		StyleableElement() = default;
		StyleableElement(const StyleableElement&) = default;
		StyleableElement(StyleableElement&&) = default;
	};
}