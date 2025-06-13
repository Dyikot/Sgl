#pragma once

#include "Trigger.h"

namespace Sgl
{
	class StyleableElement: public IStylable
	{
	public:
		Style Style;
	public:
		StyleableElement():
			Style(*this)
		{}

		virtual ~StyleableElement() = default;

		StyleableElement(const StyleableElement& other):
			Style(other.Style)
		{}

		StyleableElement(StyleableElement&& other) noexcept:
			Style(std::move(other.Style))
		{}

		const Sgl::Style& GetStyle() const { return Style; }
	};
}