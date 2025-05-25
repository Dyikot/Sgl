#pragma once

#include "../Style/Style.h"
#include "../Graphic/RenderContext.h"
#include "../Graphic/Cursor.h"

namespace Sgl
{
	class VisualElement : public IStyleable
	{
	public:
		Cursor::Getter Cursor = Cursors::Arrow;
		Color BackgroundColor = Colors::Black;
		Texture* BackgroundTexture = nullptr;
	public:
		virtual ~VisualElement() = default;

		virtual void OnRender(RenderContext rc) const = 0;

		void ResetStyle() override
		{
			Cursor = Cursors::Arrow;
			BackgroundColor = Colors::Black;
			BackgroundTexture = nullptr;
		}
	};
}