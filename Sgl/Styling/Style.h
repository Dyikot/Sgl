#pragma once

#include "../Base/Ref.h"
#include "VisualState.h"

namespace Sgl
{
	class StyleContext;

	//! @brief Abstract base class representing a visual style applied to Styleable elements
	class Style : public RefCounted
	{
	public:
		//! @brief Applies the default, unconditional base styling rules
		//! @param context The style context providing access to the target object and resources
		virtual void OnBase(StyleContext& context) = 0;

		//! @brief Applies state-specific styling rules based on the current element state
		//! @param context The style context providing access to the target object and resources
		//! @param states The set of active visual states
		virtual void OnState(StyleContext& context, const VisualStateSet& states) = 0;
	};
}