#include "UIElement.h"

namespace Sgl
{
	UIElement::UIElement():
		Class(Style)
	{
		Class.AddTarget(StyleTarget::Element);
	}
}