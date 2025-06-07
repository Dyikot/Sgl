#pragma once

#include "../Style/Style.h"
#include "../Visual/RenderContext.h"
#include "../Visual/Cursor.h"

namespace Sgl
{
	class VisualElement: public IStyleable
	{
	public:
		Cursor::Getter Cursor = Cursors::Arrow;
		Color BackgroundColor = Colors::Black;
		Texture* BackgroundTexture = nullptr;
	public:
		VisualElement() = default;
		VisualElement(const VisualElement& other);
		VisualElement(VisualElement&& other) noexcept;
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