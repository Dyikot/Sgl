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

		//! @brief Gets style collections from this host and all its ancestors
		//! @return Vector of style collections
		virtual std::vector<const StyleCollection*> GetAllStyles() const = 0;
	};
}