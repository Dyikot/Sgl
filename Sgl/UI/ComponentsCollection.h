#pragma once

#include <set>
#include "../Graphic/Cursor.h"
#include "../Graphic/IVisual.h"
#include "../Events/UIElementEvents.h"
#include "../Data/Nullable.h"

namespace Sgl
{
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
		UIElement& Parent;
		Component* HoverComponent = nullptr;
	public:
		explicit ComponentsCollection(UIElement& parent);

		void OnMouseMove(const MouseButtonEventArgs& e);
		void OnMouseDown(const MouseButtonEventArgs& e);
		void OnMouseUp(const MouseButtonEventArgs& e);
		void OnRender(RenderContext renderContext) const override;
	private:
		void SetCursor(CursorGetter cursorGetter);
	};
}