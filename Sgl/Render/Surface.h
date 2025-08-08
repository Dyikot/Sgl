#pragma once

#include <SDL/SDL_image.h>
#include <string_view>
#include <string>
#include "../Base/Size.h"
#include "../Base/Media/Color.h"
#include "../Base/Media/Font.h"

namespace Sgl
{
	/// <summary>
	/// Represents a 2D surface that can be used for rendering graphics and text.
	/// The Surface class wraps an SDL_Surface and provides a convenient interface
	/// for loading images, rendering text, and manipulating surface properties.
	/// The class automatically manages the lifetime of the underlying SDL_Surface,
	/// freeing the resources when the Surface object is destroyed.
	/// </summary>
	class Surface
	{
	private:
		SDL_Surface* _surface;
	public:
		/// <summary>
		/// Creates a surface by loading an image from the specified file path.
		/// </summary>
		/// <param name="path">- the file path to the image file to load.</param>
		explicit Surface(std::string_view path);

		/// <summary>
		/// Creates a surface by taking ownership of an existing SDL_Surface pointer.
		/// </summary>
		/// <param name="surface">- pointer to the existing SDL_Surface to take ownership of.</param>
		explicit Surface(SDL_Surface* surface);

		/// <summary>
		/// Creates a surface with rendered text using the specified font and colors.
		/// </summary>
		/// <param name="renderType">- the type of text rendering to use (solid, shaded, etc).</param>
		/// <param name="font">- TTF font to use for rendering the text.</param>
		/// <param name="text">- the text to render.</param>
		/// <param name="foreground">- the foreground color for the text.</param>
		/// <param name="background">- the background color for the text (default is white). 
		/// Only used for shaded rendering type.</param>
		Surface(FontRenderType renderType, TTF_Font* font, std::string_view text,
				Color foreground, Color background = Colors::White);

		/// <summary>
		/// Creates a surface with rendered text using the specified font, colors, and word wrapping.
		/// </summary>
		/// <param name="renderType">- the type of text rendering to use (solid, shaded, etc).</param>
		/// <param name="font">- TTF font to use for rendering the text.</param>
		/// <param name="text">- the text to render.</param>
		/// <param name="wrapLength">- the maximum width in pixels before wrapping words to the next line.</param>
		/// <param name="foreground">- the foreground color for the text.</param>
		/// <param name="background">- the background color for the text (default is white). 
		/// Only used for shaded rendering type.</param>
		Surface(FontRenderType renderType, TTF_Font* font,
				std::string_view text, unsigned int wrapLength,
				Color foreground, Color background = Colors::White);

		Surface(const Surface&) = delete;
		Surface(Surface&& other) noexcept;
		~Surface();

		/// <summary>
		/// Gets the size dimensions of the surface.
		/// </summary>
		/// <returns>Size object containing the width and height of the surface.</returns>
		Size GetSize() const 
		{
			return Size(_surface->w, _surface->h);
		}

		/// <summary>
		/// Gets the underlying SDL_Surface pointer.
		/// </summary>
		/// <returns>Pointer to the internal SDL_Surface object.</returns>
		SDL_Surface* GetSDLSurface() const noexcept
		{ 
			return _surface;
		}

		/// <summary>
		/// Sets the color modulation for the surface.
		/// </summary>
		/// <param name="color">- the color to modulate the surface with.</param>
		void SetColor(Color color)
		{
			SDL_SetSurfaceColorMod(_surface, color.Red, color.Green, color.Blue);
			SDL_SetSurfaceAlphaMod(_surface, color.Alpha);
		}

		Surface& operator=(const Surface&) = delete;
		Surface& operator=(Surface&& other) noexcept;
	};
}