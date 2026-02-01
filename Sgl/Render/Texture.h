#pragma once

#include <SDL3/SDL_render.h>

#include "../Base/Primitives.h"
#include "../Base/Media/Font.h"
#include "../Base/Size.h"

namespace Sgl
{
	class Surface;

	/// <summary>
	/// Specifies how a texture is intended to be used, which affects performance and memory layout.
	/// </summary>
	enum class TextureAccess
	{
		Static,    // The texture is rarely updated and optimized for fast rendering (e.g., sprites, UI assets).
		Streaming, // The texture is frequently updated from CPU memory (e.g., dynamic buffers, video frames).
		Target     // The texture can be used as a render target (i.e., rendered into via SetTarget).
	};
	
	/// <summary>
	/// A RAII wrapper around SDL_Texture that manages a reference-counted GPU texture.
	/// </summary>
	class Texture final
	{
	private:
		SDL_Texture* _texture = nullptr;
	public:
		/// <summary>
		/// Constructs an empty (null) texture.
		/// </summary>
		Texture() = default;

		/// <summary>
		/// Constructs a null texture from a nullptr literal.
		/// </summary>
		Texture(std::nullptr_t);

		/// <summary>
		/// Loads a texture from an image file using the given renderer.
		/// </summary>
		/// <param name="renderer"> - the SDL renderer used to create the texture.</param>
		/// <param name="filePath"> - path to the image file.</param>
		Texture(SDL_Renderer* renderer, std::string_view filePath);

		/// <summary>
		/// Creates a texture by uploading the pixel data from a surface.
		/// </summary>
		/// <param name="renderer"> - the SDL renderer used to create the texture.</param>
		/// <param name="surface"> - the surface whose contents will be copied into the texture.</param>
		Texture(SDL_Renderer* renderer, const Surface& surface);

		/// <summary>
		/// Creates an empty texture with specified dimensions, access type, and pixel format.
		/// </summary>
		/// <param name="renderer"> - the SDL renderer used to create the texture.</param>
		/// <param name="size"> - width and height of the new texture.</param>
		/// <param name="access"> - how the texture will be used (static, streaming, or render target). Defaults to Static.</param>
		/// <param name="format"> - pixel format of the texture. Defaults to SDL_PIXELFORMAT_RGBA8888.</param>
		Texture(SDL_Renderer* renderer,
				Size size,
				TextureAccess access = TextureAccess::Static,
				SDL_PixelFormat format = SDL_PIXELFORMAT_RGBA8888);

		/// <summary>
		/// Renders text to a texture using the given font and colors.
		/// </summary>
		/// <param name="renderer"> - the SDL renderer used to create the texture.</param>
		/// <param name="fontQuality"> - rendering quality (e.g., solid, shaded, blended).</param>
		/// <param name="font"> - pointer to the loaded TrueType font.</param>
		/// <param name="text"> - text to render.</param>
		/// <param name="foreground"> - color of the text.</param>
		/// <param name="background"> - background color; defaults to transparent.</param>
		Texture(SDL_Renderer* renderer,
				FontQuality fontQuality,
				TTF_Font* font,
				std::string_view text,
				Color foreground,
				Color background = Colors::Transparent);

		/// <summary>
		/// Renders wrapped text to a texture using the given font, colors, and maximum line width.
		/// </summary>
		/// <param name="renderer"> - the SDL renderer used to create the texture.</param>
		/// <param name="fontQuality"> - rendering quality (e.g., solid, shaded, blended).</param>
		/// <param name="font"> - pointer to the loaded TrueType font.</param>
		/// <param name="text"> - text to render.</param>
		/// <param name="wrapWidth"> - maximum width in pixels before wrapping lines.</param>
		/// <param name="foreground"> - color of the text.</param>
		/// <param name="background"> - background color; defaults to transparent.</param>
		Texture(SDL_Renderer* renderer,
				FontQuality fontQuality,
				TTF_Font* font,
				std::string_view text,
				int wrapWidth,
				Color foreground,
				Color background = Colors::Transparent);

		/// <summary>
		/// Copy constructor. Increments the reference count of the underlying SDL_Texture.
		/// </summary>
		/// <param name="other"> - the texture to copy from.</param>
		Texture(const Texture& other);

		/// <summary>
		/// Move constructor. Transfers ownership without modifying the reference count.
		/// </summary>
		/// <param name="other"> - the texture to move from.</param>
		Texture(Texture&& other) noexcept;

		/// <summary>
		/// Destructor. Decrements the reference count of the underlying SDL_Texture.
		/// The GPU resource is freed only when the reference count reaches zero.
		/// </summary>
		~Texture();

		/// <summary>
		/// Sets the color modulation (multiply color) applied during rendering.
		/// </summary>
		/// <param name="value"> - the color to modulate with.</param>
		void SetColor(Color value);

		/// <summary>
		/// Gets the current color modulation of the texture.
		/// </summary>
		/// <returns>The color currently used for modulation.</returns>
		Color GetColor() const;

		/// <summary>
		/// Sets the blend mode used when rendering this texture.
		/// </summary>
		/// <param name="value"> - the SDL_BlendMode to apply (e.g., blend, add, modulate).</param>
		void SetBlendMode(SDL_BlendMode value);

		/// <summary>
		/// Gets the current blend mode of the texture.
		/// </summary>
		/// <returns>The blend mode currently set for this texture.</returns>
		SDL_BlendMode GetBlendMode() const;

		/// <summary>
		/// Sets the scale quality (filtering) used when the texture is scaled during rendering.
		/// </summary>
		/// <param name="value"> - the scaling mode.</param>
		void SetScaleMode(SDL_ScaleMode value);

		/// <summary>
		/// Gets the current scale mode of the texture.
		/// </summary>
		/// <returns>The scale filtering mode currently in use.</returns>
		SDL_ScaleMode GetScaleMode() const;

		/// <summary>
		/// Gets the width of the texture in pixels.
		/// </summary>
		/// <returns>Width of the texture.</returns>
		uint32_t GetWidth() const;

		/// <summary>
		/// Gets the height of the texture in pixels.
		/// </summary>
		/// <returns>Height of the texture.</returns>
		uint32_t GetHeight() const;

		/// <summary>
		/// Gets the access type of the texture (Static, Streaming, or Target).
		/// </summary>
		/// <returns>The access mode specified at creation time.</returns>
		TextureAccess GetAccess() const;

		/// <summary>
		/// Gets the pixel format of the texture.
		/// </summary>
		/// <returns>The SDL pixel format used by this texture.</returns>
		SDL_PixelFormat GetFormat() const;

		/// <summary>
		/// Gets the raw SDL_Texture pointer managed by this object.
		/// </summary>
		/// <returns>Pointer to the underlying SDL_Texture, or nullptr if empty.</returns>
		SDL_Texture* GetSDLTexture() const noexcept;

		/// <summary>
		/// Assigns a null state to this texture, releasing its reference.
		/// </summary>
		/// <returns>Reference to this texture after assignment.</returns>
		Texture& operator=(std::nullptr_t);

		/// <summary>
		/// Copy assignment operator. Releases current reference and increments the reference count of the source.
		/// </summary>
		/// <param name="other"> - the texture to copy from.</param>
		/// <returns>Reference to this texture after assignment.</returns>
		Texture& operator=(const Texture& other);

		/// <summary>
		/// Move assignment operator. Transfers ownership from another texture.
		/// </summary>
		/// <param name="other"> - the texture to move from.</param>
		/// <returns>Reference to this texture after assignment.</returns>
		Texture& operator=(Texture&& other) noexcept;

		/// <summary>
		/// Compares two textures for equality based on their underlying SDL_Texture pointers.
		/// </summary>
		/// <returns>True if both textures wrap the same SDL_Texture or are both null; otherwise, false.</returns>
		friend bool operator==(const Texture&, const Texture&) = default;

		/// <summary>
		/// Compares the texture with nullptr.
		/// </summary>
		/// <returns>True if the texture is null; otherwise, false.</returns>
		bool operator==(std::nullptr_t) const noexcept { return _texture == nullptr; }

		/// <summary>
		/// Checks whether the texture is valid (non-null).
		/// </summary>
		/// <returns>True if the texture owns a valid SDL_Texture; otherwise, false.</returns>
		explicit operator bool() const noexcept { return _texture != nullptr; }
	private:
		void CopyFrom(const Texture& other);
		void Destroy();
	};

	/// <summary>
	/// A RAII helper that locks a streaming texture for direct pixel access.
	/// Must only be used with textures created with TextureAccess::Streaming.
	/// The texture remains locked during the lifetime of this object.
	/// </summary>
	class TextureLock
	{
	private:
		Texture& _texture;
	public:
		/// <summary>
		/// Pointer to the raw pixel data of the locked texture region.
		/// </summary>
		void* Pixels = nullptr;

		/// <summary>
		/// Number of bytes per row (including padding) in the locked pixel data.
		/// </summary>
		int Pitch = 0;

		/// <summary>
		/// Locks the specified texture for direct pixel access.
		/// If a rectangle is provided, only that region is locked; otherwise, the entire texture is locked.
		/// </summary>
		/// <param name="texture"> - the streaming texture to lock (must be valid and unlocked).</param>
		/// <param name="rect"> - optional rectangle to lock; if null, the whole texture is locked.</param>
		TextureLock(Texture& texture, const Rect* rect = nullptr);

		/// <summary>
		/// Unlocks the texture, committing any changes made to the pixel data.
		/// </summary>
		~TextureLock();
	};

	/// <summary>
	/// A helper class that loads textures using a shared SDL renderer.
	/// Holds a reference to an external renderer and provides convenience methods for texture creation.
	/// </summary>
	class TextureLoader
	{
	public:
		/// <summary>
		/// Constructs a texture loader that uses the specified renderer.
		/// </summary>
		/// <param name="renderer"> - reference to the SDL renderer pointer used for all texture creation.</param>
		explicit TextureLoader(SDL_Renderer*& renderer):
			_renderer(renderer) 
		{}

		/// <summary>
		/// Loads a texture from an image file using the associated renderer.
		/// </summary>
		/// <param name="filePath"> - path to the image file.</param>
		/// <returns>A new Texture instance loaded from the file.</returns>
		Texture Load(std::string_view filePath)
		{
			return Texture(_renderer, filePath);
		}
	private:
		SDL_Renderer*& _renderer;
	};
}