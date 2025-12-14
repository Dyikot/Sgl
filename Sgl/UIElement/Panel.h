#pragma once

#include <set>

#include "UIElementsCollection.h"

namespace Sgl
{
	class Panel : public UIElement
	{
	public:
		UIElementsCollection Children;
	private:
		Ref<UIElement> _currentChild;
	public:
		Panel();
		Panel(const Panel& other);
		Panel(Panel&& other) noexcept;
		~Panel() = default;

		void SetVisualRoot(IVisualRoot* value) override;

		void Render(RenderContext context) final;
		void ApplyStyle() override;
	protected:
		void OnAttachedToLogicalTree() override;
		void OnDetachedFromLogicalTree() override;
		void OnCursorChanged(const Cursor& cursor) override;
		void OnMouseMove(MouseMoveEventArgs e) override;
		void OnMouseDown(MouseButtonEventArgs e) override;
		void OnMouseUp(MouseButtonEventArgs e) override;
		void OnMouseLeave(MouseMoveEventArgs e) override;
	};
}