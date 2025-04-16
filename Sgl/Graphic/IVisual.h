#pragma once

#include "RenderContext.h"

namespace Sgl
{
	class IVisual
	{
	public:
		virtual ~IVisual() = default;

		virtual void OnRender(RenderContext renderContext) const = 0;
	};
}