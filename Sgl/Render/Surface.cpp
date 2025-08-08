#include "Surface.h"
#include "../Base/Log.h"

namespace Sgl
{

	Surface::Surface(std::string_view path):
		_surface(IMG_Load(path.data()))
	{
		Log::PrintSDLErrorIf(_surface == nullptr);
	}

	Surface::Surface(SDL_Surface* surface):
		_surface(surface)
	{
		Log::PrintSDLErrorIf(_surface == nullptr);
	}

	Surface::Surface(FontRenderType renderType, TTF_Font* font, std::string_view text, 
					 Color foreground, Color background)
	{
		switch(renderType)
		{
			case Sgl::FontRenderType::Blended:
				_surface = TTF_RenderUTF8_Blended(font, text.data(), foreground);
				break;
			case Sgl::FontRenderType::Solid:
				_surface = TTF_RenderUTF8_Solid(font, text.data(), foreground);
				break;
			case Sgl::FontRenderType::Shaded:
				_surface = TTF_RenderUTF8_Shaded(font, text.data(), foreground, background);
				break;
			case Sgl::FontRenderType::LCD:
				_surface = TTF_RenderUTF8_LCD(font, text.data(), foreground, background);
				break;
		}

		Log::PrintSDLErrorIf(_surface == nullptr);
	}

	Surface::Surface(FontRenderType renderType, TTF_Font * font, std::string_view text,
					 unsigned int wrapLength, Color foreground, Color background)
	{
		switch(renderType)
		{
			case Sgl::FontRenderType::Blended:
				_surface = TTF_RenderUTF8_Blended_Wrapped(font, text.data(), foreground, wrapLength);
				break;
			case Sgl::FontRenderType::Solid:
				_surface = TTF_RenderUTF8_Solid_Wrapped(font, text.data(), foreground, wrapLength);
				break;
			case Sgl::FontRenderType::Shaded:
				_surface = TTF_RenderUTF8_Shaded_Wrapped(font, text.data(), foreground, background, wrapLength);
				break;
			case Sgl::FontRenderType::LCD:
				_surface = TTF_RenderUTF8_LCD_Wrapped(font, text.data(), foreground, background, wrapLength);
				break;
		}

		Log::PrintSDLErrorIf(_surface == nullptr);
	}

	Surface::Surface(Surface && other) noexcept:
		_surface(std::exchange(other._surface, nullptr))
	{}

	Surface::~Surface()
	{
		SDL_FreeSurface(_surface);
	}

	Surface& Surface::operator=(Surface&& other) noexcept
	{
		_surface = std::exchange(other._surface, nullptr);
		return *this;
	}
}
