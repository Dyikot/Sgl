#pragma once

struct SDL_Renderer;

namespace Sgl
{
    class IVisualRoot
    {
    public:
        virtual ~IVisualRoot() = default;

        virtual SDL_Renderer* GetRenderer() const = 0;
        virtual void InvalidateRender() = 0;
        virtual bool NeedsRendering() const = 0;
    };
}