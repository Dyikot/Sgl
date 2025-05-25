#pragma once

#include "../Style/Font.h"
#include "UIElement.h"

namespace Sgl::UI
{
	class TextElement: public UIElement
	{
	private:
		using base = UIElement;
	public:
		std::string Text;
		TextDecoration TextDecoration = TextDecoration::None;
		Font Font;
	public:
		void ResetStyle() override
		{
			base::ResetStyle();

			Text = {};
			TextDecoration = TextDecoration::None;
			Font = {};
		}
	};
}