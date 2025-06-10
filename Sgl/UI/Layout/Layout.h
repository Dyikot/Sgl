#pragma once

#include <set>
#include <unordered_map>
#include <memory>
#include "../UIElement.h"
#include "../../Data/Ranges/Ranges.h"
#include "../../Data/Size.h"
#include "../../Data/Ranges/SortedVector.h"

namespace Sgl
{
	class Scene;
}

namespace Sgl::UI
{
	class Layout: public UIElement
	{
	public:
		using UIElementCollection = SortedVector<std::shared_ptr<UIElement>, UIElementComparer>;

		VisualElement& Parent;
	protected:
		UIElementCollection _children;
		bool _shouldArrange;
	private:
		using base = UIElement;

		bool _isHover;
		std::shared_ptr<UIElement> _hoverChild;
	public:
		Layout(VisualElement& parent);

		const UIElementCollection& Children() const { return _children; }

		void QueryArrange();
		void OnRender(RenderContext rc) const override;
		void Add(const std::shared_ptr<UIElement>& element);
		void Remove(const std::shared_ptr<UIElement>& element);
	protected:
		static void SetElementPosition(UIElement& element, FPoint position) { element.SetPosition(position); }
		static FPoint GetElementPosition(UIElement& element) { return element.GetPosition(); }

		virtual void Arrange() = 0;

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