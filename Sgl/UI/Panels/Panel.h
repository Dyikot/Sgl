#pragma once

#include "../Component.h"
#include "../ComponentSet.h"
#include "../Controls/Control.h"

namespace Sgl
{
	class Panel: public Component
	{
	public:
		Control* MouseOverControl = nullptr;
		ComponentSet<Control> Children;	
	public:
		Panel() = default;

		void SetStyle(const Style& style) override;

		bool TryRaiseMouseMoveEvents(const MouseButtonEventArgs& e);
		void RaiseMouseDownEvents(const MouseButtonEventArgs& e);
		void RaiseMouseUpEvents(const MouseButtonEventArgs& e);
	};	
}