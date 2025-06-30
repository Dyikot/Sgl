#pragma once

#include <vector>
#include <unordered_map>
#include "Style.h"

namespace Sgl
{
	class StyleableElement
	{
	public:
		StyleCollection Styles;
		std::vector<std::string> Classes;
	public:
		StyleableElement() = default;
		StyleableElement(const StyleableElement&) = default;
		StyleableElement(StyleableElement&&) = default;
		virtual ~StyleableElement() = default;

		virtual void ApplyStyle();
	};
}