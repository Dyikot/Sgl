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

		//! @brief Merges the styles hosted by this object into a target style
		//! @param element The styleable element providing context for the style resolution
		//! @param target The target style object that will receive the merged setters
		virtual void MergeStylesTo(Styleable& element, Style& target) = 0;
	};
}