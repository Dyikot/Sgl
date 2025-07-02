#pragma once

#include "RenderContext.h"

namespace Sgl
{
    class IRenderable
    {
    public:
        virtual ~IRenderable() = default;

        virtual void OnRender(RenderContext context) const = 0;
    };
}