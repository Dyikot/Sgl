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
		UIElement* _currentChild;
	public:
		Panel();
		Panel(const Panel& other);
		Panel(Panel&& other) noexcept;

		void Render(RenderContext context) override;
		void ApplyStyle() override;
	protected:
		void OnMouseMove(const MouseEventArgs& e) override;
		void OnMouseDown(const MouseButtonEventArgs& e) override;
		void OnMouseUp(const MouseButtonEventArgs& e) override;
		void OnMouseLeave(const MouseEventArgs& e) override;
	};
}