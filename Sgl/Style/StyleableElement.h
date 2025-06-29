#pragma once

#include <vector>
#include <unordered_map>
#include "Style.h"
#include "../Data/Object.h"

namespace Sgl
{
	class StyleableElement
	{
	public:
		std::unordered_map<std::string, shared_ptr<IStyle>> Styles;
		std::vector<std::string> Classes;
	public:
		StyleableElement() = default;
		StyleableElement(const StyleableElement&) = default;
		StyleableElement(StyleableElement&&) = default;
		virtual ~StyleableElement() = default;

		virtual void ApplyStyle();
	};
}