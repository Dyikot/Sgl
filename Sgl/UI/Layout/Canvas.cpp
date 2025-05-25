#include "Canvas.h"

namespace Sgl::UI
{
	std::unique_ptr<Canvas> Canvas::New(VisualElement& parent)
	{
		return std::make_unique<Canvas>(parent);
	}

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