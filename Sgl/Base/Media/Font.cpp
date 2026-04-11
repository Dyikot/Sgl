#include "Font.h"
#include "../Logging.h"

#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_platform_defines.h>
#include <filesystem>

namespace fs = std::filesystem;

namespace Sgl
{
	#ifdef SDL_PLATFORM_WIN32
	static constexpr auto FamiliesPath = "C:/Windows/Fonts/";
	#elif SDL_PLATFORM_LINUX
	static constexpr auto FamiliesPath = "/usr/share/fonts/";
	#elif SDL_PLATFORM_MACOS
	static constexpr auto FamiliesPath = "/System/Library/Fonts/";
	#endif

	static constexpr auto DefaultFamily = "segoeui.ttf";
	static const FontFamily DefaultFontFamily { FamiliesPath, DefaultFamily };

	static inline std::string NormalizePath(const fs::path& path)
	{
		return path.lexically_normal().generic_string();
	}

	FontFamily::FontFamily(DefaultTag):
		FontFamily(DefaultFontFamily)
	{}

	FontFamily::FontFamily(const std::string& name):
		FontFamily(FamiliesPath, name)
	{}

	FontFamily::FontFamily(const std::string& path, const std::string& name):
		_impl(new FontFamilyImpl(NormalizePath(fs::path(path) / name), 1))
	{}

	FontFamily::FontFamily(const FontFamily& other):
		_impl(other._impl)
	{
		Acquire();
	}

	FontFamily::FontFamily(FontFamily&& other) noexcept:
		_impl(other._impl)
	{
		other._impl = nullptr;
	}

	FontFamily::~FontFamily()
	{
		Release();
	}

	const std::string& FontFamily::GetSource() const
	{
		return _impl->Source;
	}

	FontFamily& FontFamily::operator=(const FontFamily& other)
	{
		Release();
		_impl = other._impl;
		Acquire();
		return *this;
	}

	FontFamily& FontFamily::operator=(FontFamily&& other) noexcept
	{
		_impl = other._impl;
		other._impl = nullptr;
		return *this;
	}

	void FontFamily::Acquire()
	{
		if(_impl)
		{
			_impl->References++;
		}
	}

	void FontFamily::Release()
	{
		if(_impl && --(_impl->References) == 0)
		{
			delete _impl;
			_impl = nullptr;
		}
	}

	TrueTypeFont::TrueTypeFont(const FontFamily& fontFamily, float size):
		_font(TTF_OpenFont(fontFamily.GetSource().data(), size))
	{
		if(_font == nullptr)
		{
			Logging::LogError("Unable to create a font: {}", SDL_GetError());
		}
	}

	TrueTypeFont::TrueTypeFont(TrueTypeFont&& other) noexcept:
		_font(std::exchange(other._font, nullptr))
	{}

	TrueTypeFont::~TrueTypeFont()
	{
		if(_font)
		{
			TTF_CloseFont(_font);
		}
	}

	void TrueTypeFont::SetSize(float size)
	{
		TTF_SetFontSize(_font, size);
	}

	void TrueTypeFont::SetStyle(FontStyle fontStyle)
	{
		TTF_SetFontStyle(_font, static_cast<unsigned long>(fontStyle));
	}

	void TrueTypeFont::SetOutline(int outline)
	{
		TTF_SetFontOutline(_font, outline);
	}

	void TrueTypeFont::SetFlowDirection(FlowDirection flowDirection)
	{
		auto ttfDirection = static_cast<TTF_Direction>(static_cast<int>(flowDirection) + 4);
		TTF_SetFontDirection(_font, ttfDirection);
	}

	void TrueTypeFont::SetTextAligment(TextAlignment textAlignment)
	{
		TTF_SetFontWrapAlignment(_font, TTF_HorizontalAlignment(textAlignment));
	}

	TrueTypeFont& TrueTypeFont::operator=(TrueTypeFont&& other) noexcept
	{
		_font = std::exchange(other._font, _font);
		return *this;
	}
}
