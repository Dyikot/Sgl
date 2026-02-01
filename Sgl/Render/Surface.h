#pragma once

#include <string_view>
#include <SDL3/SDL_surface.h>

#include "../Base/Size.h"
#include "../Base/Media/Color.h"

namespace Sgl
{
	/// <summary>
	/// A RAII wrapper around SDL_Surface that manages a reference-counted surface.
	/// </summary>
	class Surface final
	{
	private:
		SDL_Surface* _surface = nullptr;
	public:
		/// <summary>
		/// Constructs an empty (null) surface.
		/// </summary>
		Surface() = default;

		/// <summary>
		/// Constructs a null surface from a nullptr literal.
		/// </summary>
		Surface(std::nullptr_t);

		/// <summary>
		/// Loads a surface from an image file at the specified path.
		/// </summary>
		/// <param name="path"> - path to the image file to load.</param>
		explicit Surface(std::string_view path);

		/// <summary>
		/// Takes ownership of an existing SDL_Surface pointer.
		/// </summary>
		/// <param name="sdlSurface"> - raw pointer to an SDL_Surface.</param>
		explicit Surface(SDL_Surface* sdlSurface);

		/// <summary>
		/// Creates a new surface with the specified size and pixel format.
		/// </summary>
		/// <param name="size"> - width and height of the new surface.</param>
		/// <param name="format"> - pixel format for the new surface.</param>
		Surface(Size size, SDL_PixelFormat format);

		/// <summary>
		/// Creates a surface that references external pixel data with custom pitch.
		/// </summary>
		/// <param name="size"> - dimensions of the surface.</param>
		/// <param name="format"> - pixel format of the surface.</param>
		/// <param name="pixels"> - pointer to pre-allocated pixel data.</param>
		/// <param name="pitch"> - number of bytes per row (including padding).</param>
		Surface(Size size, SDL_PixelFormat format, void* pixels, int pitch);

		/// <summary>
		/// Copy constructor. Increments the reference count of the underlying SDL_Surface.
		/// </summary>
		/// <param name="other"> - the surface to copy from.</param>
		Surface(const Surface& other);

		/// <summary>
		/// Move constructor. Transfers ownership without modifying the reference count.
		/// </summary>
		/// <param name="other"> - the surface to move from.</param>
		Surface(Surface&& other) noexcept;

		/// <summary>
		/// Destructor. Decrements the reference count of the underlying SDL_Surface.
		/// The surface data is freed only when the reference count reaches zero.
		/// </summary>
		~Surface();

		/// <summary>
		/// Sets the color key for the surface.
		/// </summary>
		/// <param name="color"> - the color.</param>
		void SetColor(Color color);

		/// <summary>
		/// Gets the current color key of the surface.
		/// </summary>
		/// <returns>The color currently set.</returns>
		Color GetColor() const;

		/// <summary>
		/// Sets the blend mode used for blitting this surface.
		/// </summary>
		/// <param name="value"> - the SDL_BlendMode to apply during rendering.</param>
		void SetBlendMode(SDL_BlendMode value);

		/// <summary>
		/// Gets the current blend mode of the surface.
		/// </summary>
		/// <returns>The blend mode currently set for this surface.</returns>
		SDL_BlendMode GetBlendMode() const;

		/// <summary>
		/// Gets the width of the surface in pixels.
		/// </summary>
		/// <returns>Width of the surface.</returns>
		uint32_t GetWidth() const;

		/// <summary>
		/// Gets the height of the surface in pixels.
		/// </summary>
		/// <returns>Height of the surface.</returns>
		uint32_t GetHeight() const;

		/// <summary>
		/// Gets the raw SDL_Surface pointer managed by this object.
		/// </summary>
		/// <returns>Pointer to the underlying SDL_Surface, or nullptr if empty.</returns>
		SDL_Surface* GetSDLSurface() const noexcept { return _surface; }

		/// <summary>
		/// Locks the surface pixel buffer for direct access.
		/// Must be paired with Unlock().
		/// </summary>
		void Lock();

		/// <summary>
		/// Unlocks the surface pixel buffer after direct access.
		/// </summary>
		void Unlock();

		/// <summary>
		/// Flips the surface contents horizontally, vertically, or both.
		/// </summary>
		/// <param name="flipMode"> - specifies the flip orientation.</param>
		void Flip(SDL_FlipMode flipMode);

		/// <summary>
		/// Creates a deep copy of this surface.
		/// The new surface has its own pixel data and reference count.
		/// </summary>
		/// <returns>A new Surface instance containing a copy of this surface's data.</returns>
		Surface Clone() const;

		/// <summary>
		/// Assigns a null state to this surface, releasing its reference.
		/// </summary>
		/// <returns>Reference to this surface after assignment.</returns>
		Surface& operator=(std::nullptr_t);

		/// <summary>
		/// Copy assignment operator. Releases current reference and increments the reference count of the source.
		/// </summary>
		/// <param name="other"> - the surface to copy from.</param>
		/// <returns>Reference to this surface after assignment.</returns>
		Surface& operator=(const Surface& other);

		/// <summary>
		/// Move assignment operator. Transfers ownership from another surface.
		/// </summary>
		/// <param name="other"> - the surface to move from.</param>
		/// <returns>Reference to this surface after assignment.</returns>
		Surface& operator=(Surface&& other) noexcept;

		/// <summary>
		/// Compares two surfaces for equality based on their underlying SDL_Surface pointers.
		/// </summary>
		/// <returns>True if both surfaces wrap the same SDL_Surface or are both null; otherwise, false.</returns>
		friend bool operator==(const Surface&, const Surface&) = default;

		/// <summary>
		/// Compares the surface with nullptr.
		/// </summary>
		/// <returns>True if the surface is null; otherwise, false.</returns>
		bool operator==(std::nullptr_t) const noexcept { return _surface == nullptr; }

		/// <summary>
		/// Checks whether the surface is valid (non-null).
		/// </summary>
		/// <returns>True if the surface owns a valid SDL_Surface; otherwise, false.</returns>
		explicit operator bool() const noexcept { return _surface != nullptr; }
	private:
		void CopyFrom(const Surface& other);
		void Destroy();
	};
}