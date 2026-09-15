#pragma once

#include "StyleCollection.h"

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

		//! @brief Gets the parent style host used for style inheritance and resolution
		//! @return A pointer to the parent style host, or nullptr if there is no parent
		virtual IStyleHost* GetStylingParent() = 0;
	};
}