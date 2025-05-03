#pragma once

#include "UIElement.h"
#include "ComponentsCollection.h"

namespace Sgl
{
	using ComponentEventHandler = EventHandler<Component, EventArgs>;

	class Component: public UIElement
	{
	public:
		SDL_FPoint position = { 0, 0 };
		ComponentsCollection children = ComponentsCollection(*this);
		Event<MouseEventHandler> mouseEnter;
		Event<MouseEventHandler> mouseLeave;
	private:
		bool _hover = false;
	public:
		Component() = default;
		virtual ~Component() = default;		

		void OnRender(RenderContext renderContext) const override;

		bool IsPointIn(SDL_FPoint point) const
		{
			return point.x >= position.x &&
				   point.x <= position.x + style->width &&
				   point.y >= position.y &&
				   point.y <= position.y + style->height;
		}
	protected:
		virtual void OnMouseEnter(const MouseButtonEventArgs& e);
		virtual void OnMouseLeave(const MouseButtonEventArgs& e);

		friend class ComponentsCollection;
	};
}