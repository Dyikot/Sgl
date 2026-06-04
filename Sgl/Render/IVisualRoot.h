#pragma once

#include "Texture.h"

namespace Sgl
{
    /// <summary>
    /// Represents an interface for a visual root object that manages rendering state and provides access to the underlying renderer.
    /// </summary>
    class IVisualRoot
    {
    public:
        virtual ~IVisualRoot() = default;

        /// <summary>
        /// Gets the SDL renderer associated with this visual root.
        /// </summary>
        /// <returns>A pointer to the SDL_Renderer used for drawing.</returns>
        virtual SDL_Renderer* GetRenderer() const = 0;

        /// <summary>
        /// Provides access to the texture factory instance.
        /// </summary>
        /// <returns>A reference to the active ITextureFactory implementation.</returns>
        virtual ITextureFactory& GetTextureFactory() = 0;

        /// <summary>
        /// Marks the visual content as dirty, indicating that a re-render is required.
        /// </summary>
        virtual void InvalidateRender() = 0;
    };
}