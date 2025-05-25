#pragma once

#include "Layout.h"

namespace Sgl::UI
{
	class Canvas: public Layout
	{
	private:
		using base = Layout;
	public:
		Canvas(VisualElement& parent);

		void SetOffset(UIElement& element, Thickness value);
		Thickness GetOffset(UIElement& element);

		void Measure() override;
		void Arrange() override;
	};
}