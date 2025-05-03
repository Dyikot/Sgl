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

		template<CAction<Style&>... TStyleSelector>
		Component(TStyleSelector&&... styleSelectors)
		{
			(std::forward<TStyleSelector>(styleSelectors)(*style), ...);
		}

		virtual ~Component() = default;		

		void OnRender(RenderContext renderContext) const override;
	protected:
		virtual void OnMouseEnter(const MouseButtonEventArgs& e);
		virtual void OnMouseLeave(const MouseButtonEventArgs& e);

		friend class ComponentsCollection;
	};
}