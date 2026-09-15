#pragma once

#include <span>
#include <string_view>
#include <SDL3/SDL_blendmode.h>

#include "../Base/Size.h"
#include "../Base/Media/Color.h"
#include "../Base/Primitives.h"

struct SDL_Surface;

namespace Sgl
{
	//! @brief Specifies the flip orientation
	enum class FlipMode
	{
		None,
		Horizontal,
		Vertical,
		Both
	};

	//! @brief A RAII wrapper around SDL_Surface that manages a reference-counted surface
	class Surface final
	{
	public:
		//! @brief Constructs an empty (null) surface
		Surface() = default;

		//! @brief Constructs a null surface from a nullptr literal
		Surface(std::nullptr_t);

		//! @brief Loads a surface from an image file at the specified path
		//! @param path Path to the image file to load
		explicit Surface(std::string_view path);

		//! @brief Takes ownership of an existing SDL_Surface pointer
		//! @param sdlSurface Raw pointer to an SDL_Surface
		explicit Surface(SDL_Surface* sdlSurface);

		//! @brief Creates a new surface with the specified size and pixel format
		//! @param size Width and height of the new surface
		//! @param format Pixel format for the new surface
		Surface(Size size, SDL_PixelFormat format = SDL_PIXELFORMAT_RGBA8888);

		//! @brief Creates a surface that references external pixel data with custom pitch
		//! @param size Dimensions of the surface
		//! @param format Pixel format of the surface
		//! @param pixels Pointer to pre-allocated pixel data
		//! @param pitch Number of bytes per row (including padding)
		Surface(Size size, SDL_PixelFormat format, void* pixels, int pitch);

		//! @brief Copy constructor. Increments the reference count of the underlying SDL_Surface.
		//! @param other The surface to copy from
		Surface(const Surface& other);

		//! @brief Move constructor. Transfers ownership without modifying the reference count.
		//! @param other The surface to move from
		Surface(Surface&& other) noexcept;

		//! @brief Destructor. Decrements the reference count of the underlying SDL_Surface. The surface data is freed only when the reference count reaches zero.
		~Surface();

		//! @brief Sets the color key for the surface
		//! @param color The color
		void SetColor(Color color);

		//! @brief Gets the current color key of the surface
		//! @return The color currently set
		Color GetColor() const;

		//! @brief Sets the blend mode used for blitting this surface
		//! @param value The SDL_BlendMode to apply during rendering
		void SetBlendMode(SDL_BlendMode value);

		//! @brief Gets the current blend mode of the surface
		//! @return The blend mode currently set for this surface
		SDL_BlendMode GetBlendMode() const;

		//! @brief Gets the width of the surface in pixels
		//! @return Width of the surface
		uint32_t GetWidth() const;

		//! @brief Gets the height of the surface in pixels
		//! @return Height of the surface
		uint32_t GetHeight() const;

		//! @brief Fills a specific rectangular area with a solid color
		//! @param rect A pointer to the rectangle structure. Pass nullptr to fill the entire surface.
		//! @param color The color used to fill the rectangle
		void Fill(const Rect* rect, Color color);

		//! @brief Fills a collection of rectangular areas with a solid color
		//! @param rects A span of rectangles to be filled
		//! @param color The color used to fill the rectangles
		void FillRects(std::span<const Rect> rects, Color color);

		//! @brief Locks the surface pixel buffer for direct access. Must be paired with Unlock().
		void Lock();

		//! @brief Unlocks the surface pixel buffer after direct access
		void Unlock();

		//! @brief Flips the surface contents horizontally, vertically, or both
		//! @param flipMode Specifies the flip orientation
		void Flip(FlipMode flipMode);

		//! @brief Creates a deep copy of this surface. The new surface has its own pixel data and reference count.
		//! @return A new Surface instance containing a copy of this surface's data
		Surface Clone() const;

		//! @brief Assigns a null state to this surface, releasing its reference
		//! @return Reference to this surface after assignment
		Surface& operator=(std::nullptr_t);

		//! @brief Copy assignment operator. Releases current reference and increments the reference count of the source.
		//! @param other The surface to copy from
		//! @return Reference to this surface after assignment
		Surface& operator=(const Surface& other);

		//! @brief Move assignment operator. Transfers ownership from another surface.
		//! @param other The surface to move from
		//! @return Reference to this surface after assignment
		Surface& operator=(Surface&& other) noexcept;

		//! @brief Compares two surfaces for equality based on their underlying SDL_Surface pointers
		//! @return True if both surfaces wrap the same SDL_Surface or are both null; otherwise, false
		friend bool operator==(const Surface&, const Surface&) = default;

		//! @brief Compares the surface with nullptr
		//! @return True if the surface is null; otherwise, false
		bool operator==(std::nullptr_t) const noexcept { return _surface == nullptr; }

		//! @brief Gets the raw SDL_Surface pointer managed by this object
		//! @return Pointer to the underlying SDL_Surface, or nullptr if empty
		operator SDL_Surface* () const noexcept { return _surface; }

		//! @brief Checks whether the surface is valid (non-null)
		//! @return True if the surface owns a valid SDL_Surface; otherwise, false
		explicit operator bool() const noexcept { return _surface != nullptr; }

	private:
		void Release();

	private:
		SDL_Surface* _surface = nullptr;
	};
}