#pragma once

#include <set>
#include "../IUIElement.h"
#include "../../Events/MouseAndKeyEvents.h"

namespace Sgl
{
	class UIElementsCollection:
		public std::multiset<std::reference_wrapper<IUIElement>, ZIndexComparer>,
		public IMouseEventsListener,
		public IKeyEventsListener
	{
	public:
		IUIElement* HoverElement = nullptr;
		IVisual& Parent;
	public:
		UIElementsCollection(IVisual& parent);
	protected:
		void OnMouseMove(const MouseButtonEventArgs& e) override;
		void OnMouseDown(const MouseButtonEventArgs& e) override;
		void OnMouseUp(const MouseButtonEventArgs& e) override;
		void OnMouseDoubleClick(const MouseButtonEventArgs& e) override;
		void OnMouseWheel(const MouseWheelEventArgs& e) override;
		void OnKeyDown(const KeyEventArgs& e) override;
		void OnKeyUp(const KeyEventArgs& e) override;

		friend class Application;
	};
}