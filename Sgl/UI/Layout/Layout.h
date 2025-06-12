#pragma once

#include <set>
#include <unordered_map>
#include <memory>
#include "../../Data/Size.h"
#include "UIElementsCollection.h"

namespace Sgl
{
	class Scene;
}

namespace Sgl::UI
{
	class Layout: public UIElement, public ILayout
	{
	public:
		VisualElement& Parent;
		UIElementsCollection Children;
	protected:
		bool _shouldArrange;
	private:
		using base = UIElement;

		bool _isHover;
		shared_ptr<UIElement> _hoverChild;
	public:
		Layout(VisualElement& parent);

		void QueryArrange() override;
		void OnRender(RenderContext rc) const override;
	protected:
		static void SetElementPosition(UIElement& element, FPoint position) { element.SetPosition(position); }
		static FPoint GetElementPosition(UIElement& element) { return element.GetPosition(); }

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