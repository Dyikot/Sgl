#pragma once

#include <set>
#include "UIElementsCollection.h"

namespace Sgl
{
	class Panel: public UIElement
	{
	public:
		UIElementsCollection Children;
	private:
		Ref<UIElement> _currentChild;
	public:
		Panel();
		Panel(const Panel& other);
		Panel(Panel&& other) noexcept;

		void Render(RenderContext context) override;
		void ApplyStyle() override;
	protected:
		void OnCursorChanged(const Cursor& cursor) override;
		void OnMouseMove(MouseEventArgs& e) override;
		void OnMouseDown(MouseButtonEventArgs& e) override;
		void OnMouseUp(MouseButtonEventArgs& e) override;
		void OnMouseLeave(MouseEventArgs& e) override;
	};
}