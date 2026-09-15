#pragma once

#include "Texture.h"

namespace Sgl
{
    //! @brief Represents an interface for a visual root object that manages rendering state and provides access to the underlying renderer
    class IVisualRoot
    {
    public:
        virtual ~IVisualRoot() = default;

        //! @brief Gets the SDL renderer associated with this visual root
        //! @return A pointer to the SDL_Renderer used for drawing
        virtual SDL_Renderer* GetRenderer() const = 0;

        //! @brief Provides access to the texture factory instance
        //! @return A reference to the active ITextureFactory implementation
        virtual ITextureFactory& GetTextureFactory() = 0;

        //! @brief Marks the visual content as dirty, indicating that a re-render is required
        virtual void MarkDirty() = 0;
    };
}