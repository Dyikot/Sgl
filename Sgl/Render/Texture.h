#pragma once

#include <span>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_blendmode.h>
#include "../Base/Primitives.h"
#include "../Base/Media/Color.h"
#include "../Base/Media/Font.h"
#include "../Base/Size.h"

struct SDL_Renderer;
struct SDL_Texture;

namespace Sgl
{
    class Surface;

    //! @brief Specifies how a texture is intended to be used, which affects performance and memory layout
    enum class TextureAccess
    {
        Static,    // The texture is rarely updated and optimized for fast rendering
        Streaming, // The texture is frequently updated from CPU memory
        Target     // The texture can be used as a render target
    };

    //! @brief Specifies the scale filtering mode used when rendering a texture
    enum class ScaleMode
    {
        Nearest, // Uses nearest-neighbor filtering
        Linear   // Uses linear filtering
    };

    //! @brief A RAII wrapper around SDL_Texture that manages a reference-counted GPU texture
    class Texture final
    {
    public:
        //! @brief Constructs an empty (null) texture
        Texture() = default;

        //! @brief Constructs a null texture from a nullptr literal
        Texture(std::nullptr_t);

        //! @brief Loads a texture from an image file using the given renderer
        //! @param renderer The SDL renderer used to create the texture
        //! @param filePath Path to the image file
        Texture(SDL_Renderer* renderer, std::string_view filePath);

        //! @brief Creates a texture by uploading the pixel data from a surface
        //! @param renderer The SDL renderer used to create the texture
        //! @param surface The surface whose contents will be copied into the texture
        Texture(SDL_Renderer* renderer, const Surface& surface);

        //! @brief Creates a texture by uploading PNG image data from memory
        //! @param renderer The SDL renderer used to create the texture
        //! @param data Span containing the raw bytes of a PNG file
        Texture(SDL_Renderer* renderer, std::span<const uint8_t> data);

        //! @brief Creates an empty texture with specified dimensions, access type, and pixel format
        //! @param renderer The SDL renderer used to create the texture
        //! @param size Width and height of the new texture
        //! @param access How the texture will be used (static, streaming, or render target). Defaults to Static.
        //! @param format Pixel format of the texture. Defaults to SDL_PIXELFORMAT_RGBA8888.
        Texture(SDL_Renderer* renderer,
                Size size,
                TextureAccess access = TextureAccess::Static,
                SDL_PixelFormat format = SDL_PIXELFORMAT_RGBA8888);

        //! @brief Renders text to a texture using the given font and colors
        //! @param renderer The SDL renderer used to create the texture
        //! @param fontQuality Rendering quality (e.g., solid, shaded, blended)
        //! @param font Pointer to the loaded TrueType font
        //! @param text Text to render
        //! @param foreground Color of the text
        //! @param background Background color; defaults to transparent
        Texture(SDL_Renderer* renderer,
                FontQuality fontQuality,
                TTF_Font* font,
                std::string_view text,
                Color foreground,
                Color background = Colors::Transparent);

        //! @brief Renders wrapped text to a texture using the given font, colors, and maximum line width
        //! @param renderer The SDL renderer used to create the texture
        //! @param fontQuality Rendering quality (e.g., solid, shaded, blended)
        //! @param font Pointer to the loaded TrueType font
        //! @param text Text to render
        //! @param wrapWidth Maximum width in pixels before wrapping lines
        //! @param foreground Color of the text
        //! @param background Background color; defaults to transparent
        Texture(SDL_Renderer* renderer,
                FontQuality fontQuality,
                TTF_Font* font,
                std::string_view text,
                int wrapWidth,
                Color foreground,
                Color background = Colors::Transparent);

        //! @brief Copy constructor. Increments the reference count of the underlying SDL_Texture.
        //! @param other The texture to copy from
        Texture(const Texture& other);

        //! @brief Move constructor. Transfers ownership without modifying the reference count.
        //! @param other The texture to move from
        Texture(Texture&& other) noexcept;

        //! @brief Destructor. Decrements the reference count of the underlying SDL_Texture. The GPU resource is freed only when the reference count reaches zero.
        ~Texture();

        //! @brief Sets the color modulation (multiply color) applied during rendering
        //! @param value The color to modulate with
        void SetColor(Color value);

        //! @brief Gets the current color modulation of the texture
        //! @return The color currently used for modulation
        Color GetColor() const;

        //! @brief Sets the blend mode used when rendering this texture
        //! @param value The SDL_BlendMode to apply (e.g., blend, add, modulate)
        void SetBlendMode(SDL_BlendMode value);

        //! @brief Gets the current blend mode of the texture
        //! @return The blend mode currently set for this texture
        SDL_BlendMode GetBlendMode() const;

        //! @brief Sets the scale quality (filtering) used when the texture is scaled during rendering
        //! @param value The scaling mode
        void SetScaleMode(ScaleMode value);

        //! @brief Gets the current scale mode of the texture
        //! @return The scale filtering mode currently in use
        ScaleMode GetScaleMode() const;

        //! @brief Gets the width of the texture in pixels
        //! @return Width of the texture
        uint32_t GetWidth() const;

        //! @brief Gets the height of the texture in pixels
        //! @return Height of the texture
        uint32_t GetHeight() const;

        //! @brief Gets the access type of the texture (Static, Streaming, or Target)
        //! @return The access mode specified at creation time
        TextureAccess GetAccess() const;

        //! @brief Gets the pixel format of the texture
        //! @return The SDL pixel format used by this texture
        SDL_PixelFormat GetFormat() const;

        //! @brief Gets the SDL renderer associated with this texture
        SDL_Renderer* GetRenderer() const;

        //! @brief Assigns a null state to this texture, releasing its reference
        //! @return Reference to this texture after assignment
        Texture& operator=(std::nullptr_t);

        //! @brief Copy assignment operator. Releases current reference and increments the reference count of the source.
        //! @param other The texture to copy from
        //! @return Reference to this texture after assignment
        Texture& operator=(const Texture& other);

        //! @brief Move assignment operator. Transfers ownership from another texture.
        //! @param other The texture to move from
        //! @return Reference to this texture after assignment
        Texture& operator=(Texture&& other) noexcept;

        //! @brief Compares two textures for equality based on their underlying SDL_Texture pointers
        //! @return True if both textures wrap the same SDL_Texture or are both null; otherwise, false
        friend bool operator==(const Texture&, const Texture&) = default;

        //! @brief Compares the texture with nullptr
        //! @return True if the texture is null; otherwise, false
        bool operator==(std::nullptr_t) const noexcept { return _texture == nullptr; }

        //! @brief Gets the raw SDL_Texture pointer managed by this object
        //! @return Pointer to the underlying SDL_Texture, or nullptr if empty
        operator SDL_Texture* () const noexcept
        {
            return _texture;
        }

        //! @brief Checks whether the texture is valid (non-null)
        //! @return True if the texture owns a valid SDL_Texture; otherwise, false
        explicit operator bool() const noexcept { return _texture != nullptr; }

    private:
        void Release();

    private:
        SDL_Texture* _texture = nullptr;
    };

    //! @brief A RAII helper that locks a streaming texture for direct pixel access. Must only be used with textures created with TextureAccess::Streaming. The texture remains locked during the lifetime of this object.
    class TextureLock
    {
    public:
        //! @brief Locks the specified texture for direct pixel access. If a rectangle is provided, only that region is locked; otherwise, the entire texture is locked.
        //! @param texture The streaming texture to lock (must be valid and unlocked)
        //! @param rect Optional rectangle to lock; if null, the whole texture is locked
        TextureLock(Texture texture, const Rect* rect = nullptr);

        //! @brief Unlocks the texture, committing any changes made to the pixel data
        ~TextureLock();

        //! @brief Pointer to the raw pixel data of the locked texture region
        void* Pixels = nullptr;

        //! @brief Number of bytes per row (including padding) in the locked pixel data
        int Pitch = 0;

    private:
        Texture _texture;
    };

    class ImageSource;

    //! @brief Interface for factory objects that manage the creation and lifecycle of textures
    class ITextureFactory
    {
    public:
        virtual ~ITextureFactory() = default;

        //! @brief Creates a texture from the specified source, with optional caching
        //! @param imageSource The source of the image
        //! @param cache Whether to cache the texture for future use. Defaults to true.
        //! @return The loaded texture. If the file fails to load, returns an invalid (null) texture.
        virtual Texture Create(const ImageSource& source, bool cache) = 0;
    };
}