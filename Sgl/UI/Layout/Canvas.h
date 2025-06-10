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

		void Arrange() override;
	};
}