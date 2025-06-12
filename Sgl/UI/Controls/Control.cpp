#include "Control.h"
#include <cassert>

namespace Sgl::UI
{
	Control::Control():
		_layout(),
		_font(Font::Default()),
		_borderColor(Colors::Black),
		_padding(),
		_borderThickness(1)
	{}

	Control::Control(const Control& other):
		_layout(other._layout),
		_font(other._font),
		_borderColor(other._borderColor),
		_padding(other._padding),
		_borderThickness(other._borderThickness)
	{}

	Control::Control(Control&& other) noexcept:
		_layout(std::move(other._layout)),
		_font(std::move(other._font)),
		_borderColor(other._borderColor),
		_padding(other._padding),
		_borderThickness(other._borderThickness)
	{}

	void Control::SetLayout(shared_ptr<Layout> value)
	{
		assert(value != nullptr);
		_layout = std::move(value);
		_layout->Parent = this;
	}

	shared_ptr<Layout> Control::GetLayout() const
	{
		return _layout;
	}

	void Control::ResetToDefault()
	{
		UIElement::ResetToDefault();

		_padding = {};
		_font = Font::Default();
		_borderColor = Colors::Black;
		_borderThickness = Thickness(1);
	}
}