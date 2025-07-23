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
		std::shared_ptr<UIElement> _currentChild;
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
		void OnMouseLeave(const MouseEventArgs& e) override;
	};
}