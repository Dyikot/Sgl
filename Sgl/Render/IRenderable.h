#pragma once

#include "RenderContext.h"

namespace Sgl
{
    class IRenderable
    {
    public:
        virtual ~IRenderable() = default;

        virtual void Render(RenderContext context) = 0;
    };
}