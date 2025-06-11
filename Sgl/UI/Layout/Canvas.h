#pragma once

#include "Layout.h"

namespace Sgl::UI
{
	class Canvas: public Layout
	{
	public:
		Canvas(VisualElement& parent);

		void Arrange() override;
	};
}