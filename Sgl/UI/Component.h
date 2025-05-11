#pragma once

#include "UIElement.h"
#include "ComponentsCollection.h"

namespace Sgl
{
	using ComponentEventHandler = EventHandler<Component, EventArgs>;

	class Component: public UIElement
	{
	public:
		Point Position;
		ComponentsCollection Children;
		Event<MouseEventHandler> MouseEnter;
		Event<MouseEventHandler> MouseLeave;
	private:
		bool _isHover = false;
	public:
		Component();
		virtual ~Component() = default;

		void OnRender(RenderContext renderContext) const override;
	protected:
		virtual void OnMouseEnter(const MouseButtonEventArgs& e);
		virtual void OnMouseLeave(const MouseButtonEventArgs& e);

		friend class ComponentsCollection;
	};
}