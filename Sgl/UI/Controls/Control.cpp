#include "Control.h"

namespace Sgl::UI
{
	Control::Control():
		_layout(nullptr),
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
		_layout(other._layout),
		_font(std::move(other._font)),
		_borderColor(other._borderColor),
		_padding(other._padding),
		_borderThickness(other._borderThickness)
	{}

	void Control::ResetToDefault()
	{
		base::ResetToDefault();

		_padding = {};
		_font = Font::Default();
		_borderColor = Colors::Black;
		_borderThickness = Thickness(1);
	}

	void Control::SetLayout(Layout& layout)
	{
		_layout = &layout;
	}
}