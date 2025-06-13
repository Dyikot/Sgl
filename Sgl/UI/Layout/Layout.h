#pragma once

#include <set>
#include <unordered_map>
#include <memory>
#include "../../Data/Size.h"
#include "UIElementsCollection.h"
#include "../../Events/Event.h"

namespace Sgl
{
	class Scene;
}

namespace Sgl::UI
{
	class Layout: public UIElement, public ILayout
	{
	public:
		Event<FPoint> MouseEnter;
		Event<FPoint> MouseLeave;
	protected:
		bool _shouldArrange;
	private:
		bool _isHover;
		UIElementsCollection _children;
		shared_ptr<UIElement> _hoverChild;
	public:
		Layout();

		const UIElementsCollection& GetChildren() const { return _children; }

		void QueryArrange() override;
		void OnRender(RenderContext rc) const override;
	protected:
		static void SetElementPosition(UIElement& element, FPoint position) { element.SetPosition(position); }
		static FPoint GetElementPosition(UIElement& element) { return element.GetPosition(); }

		void OnMouseEnter(FPoint position) override;
		void OnMouseLeave(FPoint position) override;
		void OnMouseMove(const MouseButtonEventArgs& e) override;
		void OnMouseDown(const MouseButtonEventArgs& e) override;
		void OnMouseUp(const MouseButtonEventArgs& e) override;
		void OnMouseWheel(const MouseWheelEventArgs& e) override;
		void OnKeyDown(const KeyEventArgs& e) override;
		void OnKeyUp(const KeyEventArgs& e) override;
		void OnSizeChanged() override;
	private:
		void OnSceneLayoutMouseMove(const MouseButtonEventArgs& e);

		friend class Sgl::Scene;
	};
}