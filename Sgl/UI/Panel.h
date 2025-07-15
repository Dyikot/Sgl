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
		shared_ptr<UIElement> _mouseOverChild;
	public:
		Panel();
		Panel(const Panel& other);
		Panel(Panel&& other) noexcept;

		void OnRender(RenderContext context) const override;
		void ApplyStyle() override;
	protected:
		void OnMouseMove(const MouseEventArgs& e) override;
		void OnMouseDown(const MouseButtonEventArgs& e) override;
		void OnMouseUp(const MouseButtonEventArgs& e) override;
		void OnMouseWheelChanged(const MouseWheelEventArgs& e) override;
		void OnKeyDown(const KeyEventArgs& e) override;
		void OnKeyUp(const KeyEventArgs& e) override;
	private:
		void OnChildrenMouseMove(const MouseEventArgs& e);
	};
}