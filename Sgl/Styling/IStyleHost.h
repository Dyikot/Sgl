#pragma once

#include "StyleCollection.h"
#include "../Base/Delegate.h"

namespace Sgl
{
	//! @brief Represents an interface for objects that can host and resolve styles
	class IStyleHost
	{
	public:
		virtual ~IStyleHost() = default;

		//! @brief Gets the collection of styles defined by this host
		//! @return A reference to the style collection
		virtual StyleCollection& GetStyles() = 0;

		//! @brief Executes an action on the style collection in a read-only context
		//! @param action The callback action to execute with the immutable style collection
		virtual void WithStyles(const Action<const StyleCollection&>& action) const = 0;
	};
}