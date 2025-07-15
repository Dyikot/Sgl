#pragma once

#include "../Panel.h"

namespace Sgl::UIElements
{
	class Canvas: public Panel
	{
	public:
		static std::shared_ptr<Canvas> New() { return std::make_shared<Canvas>(); }
		
		Canvas() = default;

		//void Arrange();
	};
}