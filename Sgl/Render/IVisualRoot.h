#pragma once

struct SDL_Renderer;

namespace Sgl
{
    class IVisualRoot
    {
    public:
        virtual ~IVisualRoot() = default;

        virtual SDL_Renderer* GetRenderer() const noexcept = 0;
    };
}