#pragma once

#include "UIElement.h"
#include "ComponentsCollection.h"

namespace Sgl
{
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
		virtual void OnMouseEnter(const MouseButtonEventArgs& e)
		{
			_isHover = true;
			Class.ApplyStyleTo(StyleTarget::Hover);
			MouseEnter.TryRaise(*this, e);
		}

		virtual void OnMouseLeave(const MouseButtonEventArgs& e)
		{
			_isHover = false;
			Class.ApplyStyleTo(StyleTarget::Element);
			MouseLeave.TryRaise(*this, e);
		}

		friend class ComponentsCollection;
	};
}