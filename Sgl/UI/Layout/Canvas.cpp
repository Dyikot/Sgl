#include "Canvas.h"

namespace Sgl::UI
{
	Canvas::Canvas(VisualElement& parent):
		base(parent)
	{}

	void Canvas::SetOffset(UIElement& element, Thickness value)
	{
		
	}

	Thickness Canvas::GetOffset(UIElement& element)
	{
		return Thickness();
	}

	void Canvas::Measure()
	{}

	void Canvas::Arrange()
	{}
}