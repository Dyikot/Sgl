#include "VisualElement.h"

namespace Sgl
{
	VisualElement::VisualElement():
		_cursor(Cursors::Arrow),
		_backgroundColor(Colors::Black),
		_backgroundTexture(nullptr)
	{}

	VisualElement::VisualElement(const VisualElement& other):
		_cursor(other._cursor),
		_backgroundColor(other._backgroundColor),
		_backgroundTexture(other._backgroundTexture)
	{}

	VisualElement::VisualElement(VisualElement&& other) noexcept:
		_cursor(other._cursor),
		_backgroundColor(other._backgroundColor),
		_backgroundTexture(std::move(other._backgroundTexture))
	{}

	void VisualElement::ResetToDefault()
	{
		_cursor = Cursors::Arrow;
		_backgroundColor = Colors::Black;
		_backgroundTexture = {};
	}
}
