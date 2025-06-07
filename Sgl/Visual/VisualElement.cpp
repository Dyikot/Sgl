#include "VisualElement.h"

namespace Sgl
{
	VisualElement::VisualElement(const VisualElement& other):
		Cursor(other.Cursor),
		BackgroundColor(other.BackgroundColor),
		BackgroundTexture(other.BackgroundTexture)
	{}

	VisualElement::VisualElement(VisualElement&& other) noexcept:
		Cursor(other.Cursor),
		BackgroundColor(other.BackgroundColor),
		BackgroundTexture(std::exchange(other.BackgroundTexture, nullptr))
	{}
}
