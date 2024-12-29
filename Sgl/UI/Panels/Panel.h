#pragma once

#include "../Object.h"
#include "../../Collections/ObjestSet.h"
#include "../Controls/Control.h"

namespace Sgl
{
	class Panel: public Object
	{
	public:
		Control* MouseOverControl = nullptr;
		ObjestSet<Control> Children;
	protected:
		Fill _background = &Colors::Black;		
	public:
		Panel() = default;

		void SetStyle(const Style& style) override;
		void SetBackground(const Fill& value) { _background = value; }

		const Fill& GetBackground() const { return _background; }

		bool TryRaiseMouseMoveEvents(const MouseButtonEventArgs& e);
		void RaiseMouseDownEvents(const MouseButtonEventArgs& e);
		void RaiseMouseUpEvents(const MouseButtonEventArgs& e);
	private:
	};	
}