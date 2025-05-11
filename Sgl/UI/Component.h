#pragma once

#include "UIElement.h"
#include "ComponentsCollection.h"

namespace Sgl
{
	using ComponentEventHandler = EventHandler<Component, EventArgs>;

	class Component: public UIElement
	{
	public:
		SDL_FPoint Position = { 0, 0 };
		ComponentsCollection Children = ComponentsCollection(*this);
		Event<MouseEventHandler> MouseEnter;
		Event<MouseEventHandler> MouseLeave;
	private:
		StyleSelector _hoverStyleSelector = EmptySelector;
		bool _isHover = false;
	public:
		virtual ~Component() = default;

		template<StyleSelector... Selectors>
		void AddHoverStyle()
		{
			_hoverStyleSelector = CombineSelectors<Selectors...>;
		}

		void OnRender(RenderContext renderContext) const override;
	protected:
		virtual void OnMouseEnter(const MouseButtonEventArgs& e);
		virtual void OnMouseLeave(const MouseButtonEventArgs& e);

		friend class ComponentsCollection;
	};
}