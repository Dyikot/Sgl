#pragma once

#include "../Layout.h"

namespace Sgl::Controls
{
	class Canvas: public Layout
	{
	public:
		static std::shared_ptr<Canvas> New() { return std::make_shared<Canvas>(); }
		
		Canvas() = default;

		void Arrange() override;
	};
}