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
		std::vector<StyleSelector> hoverSelectors;
		Event<MouseEventHandler> mouseEnter;
		Event<MouseEventHandler> mouseLeave;
	private:
		StyleSetter _hoverStyleSetter;
		bool _hover = false;
	public:
		virtual ~Component() = default;		

		template<StyleSelector... selectors>
		void AddHoverStyle()
		{
			_hoverStyleSetter = [selectors...](Style& style) { (selectors(style), ...); };
		}

		void OnRender(RenderContext renderContext) const override;
	protected:
		virtual void OnMouseEnter(const MouseButtonEventArgs& e);
		virtual void OnMouseLeave(const MouseButtonEventArgs& e);

		friend class ComponentsCollection;
	};
}