#pragma once

#include "../Style/Style.h"
#include "../Visual/RenderContext.h"
#include "../Visual/Cursor.h"

using std::shared_ptr;
using std::make_shared;

namespace Sgl
{
	class VisualElement: public IStyleable
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
		auto GetBackgroundTexture() const { return _backgroundTexture; }

		virtual void OnRender(RenderContext rc) const = 0;

		void ResetToDefault() override;
	};
}