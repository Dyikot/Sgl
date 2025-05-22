#pragma once

#include "../Graphic/RenderContext.h"
#include "../Graphic/Cursor.h"
#include "../Style/Style.h"

namespace Sgl
{
	class IVisual
	{
	public:
		virtual ~IVisual() = default;

		virtual void OnRender(RenderContext renderContext) const = 0;

		virtual void SetCursor(Cursor::Getter value) = 0;
		virtual Cursor::Getter GetCursor() const = 0;

		virtual void SetBackgroundColor(Color value) = 0;
		virtual Color GetBackgroundColor() const = 0;

		virtual void SetBackgroundTexture(Texture* value) = 0;
		virtual const Texture* GetBackgroundTexture() const = 0;
	};

	template<>
	struct StyleProperties<IVisual>
	{
		Cursor::Getter Cursor = Cursors::Arrow;
		Color BackgroundColor = Colors::Black;
		Texture* BackgroundTexture = nullptr;
	};
}