#pragma once

#include <memory>
#include "../Visual/Cursor.h"
#include "../Visual/RenderContext.h"
#include "../Style/StyleableElement.h"

using std::shared_ptr;

namespace Sgl
{
	class VisualElement: public StyleableElement
	{
	private:
		Cursor::Getter _cursor;
		Color _backgroundColor;
		shared_ptr<Texture> _backgroundTexture;
	public:
		VisualElement();
		VisualElement(const VisualElement& other);
		VisualElement(VisualElement&& other) noexcept;
		virtual ~VisualElement() = default;

		void SetCursor(Cursor::Getter value) { _cursor = value; }
		Cursor::Getter GetCursor() const { return _cursor; }

		void SetBackgroundColor(Color value) { _backgroundColor = value; }
		Color GetBackgroundColor() const { return _backgroundColor; }
		
		void SetBackgroundTexture(shared_ptr<Texture> value) { _backgroundTexture = value; }
		shared_ptr<Texture> GetBackgroundTexture() const { return _backgroundTexture; }

		virtual void OnRender(RenderContext rc) const = 0;
	};
}