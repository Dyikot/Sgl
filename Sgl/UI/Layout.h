#pragma once

#include <set>
#include "UIElementsCollection.h"
#include "../Base/Size.h"
#include "../Base/Events/Event.h"

namespace Sgl
{
	class Layout: public UIElement, public ILayout
	{
	public:
		UIElementsCollection Children;
	protected:
		bool _shouldArrange;
	private:
		bool _isHover;
		shared_ptr<UIElement> _hoverChild;
	public:
		Layout();

		void QueryArrange() override;
		void OnRender(RenderContext context) const override;
		void ApplyStyle() override;
	protected:
		static void SetPosition(UIElement& element, FPoint position) { element.SetPosition(position); }
		static FPoint GetPosition(UIElement& element) { return element.GetPosition(); }

		void OnMouseMove(const MouseEventArgs& e) override;
		void OnMouseDown(const MouseButtonEventArgs& e) override;
		void OnMouseUp(const MouseButtonEventArgs& e) override;
		void OnMouseWheelChanged(const MouseWheelEventArgs& e) override;
		void OnKeyDown(const KeyEventArgs& e) override;
		void OnKeyUp(const KeyEventArgs& e) override;
		void OnSizeChanged() override;
	private:
		void OnChildrenMouseMove(const MouseEventArgs& e);

		friend class Application;
		friend class Scene;
	};
}