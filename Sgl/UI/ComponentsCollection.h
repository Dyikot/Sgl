#pragma once

#include <set>
#include "../Graphic/Cursor.h"
#include "../Graphic/IVisual.h"
#include "../Events/EventArgs.h"
#include "../Data/Nullable.h"

namespace Sgl
{
	class UIElement;
	class Component;

	struct ZIndexComparer
	{
		bool operator()(const Component& left, const Component& right) const;
	};

	class ComponentsCollection: 
		public std::multiset<std::reference_wrapper<Component>, ZIndexComparer>,
		public IVisual
	{
	public:
		UIElement& parent;
		Nullable<Component> hoverComponent;
	public:
		explicit ComponentsCollection(UIElement& parent);

		void OnMouseMove(const MouseButtonEventArgs& e);
		void OnMouseDown(const MouseButtonEventArgs& e);
		void OnMouseUp(const MouseButtonEventArgs& e);
		void OnRender(RenderContext renderContext) const override;
	private:
		void SetCursor(const Cursor& cursor);
	};
}