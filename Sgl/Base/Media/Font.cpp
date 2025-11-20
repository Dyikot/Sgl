#include "Font.h"
#include <SDL3/SDL_log.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_platform.h>
#include <unordered_map>
#include <filesystem>

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

	static std::string GetFullPath(const std::string& path, const std::string& name)
	{
		auto basePath = std::filesystem::path(path) / name;		
		return basePath.string();
	}

	FontFamily::FontFamily(DefaultFontFamilty):
		FontFamily(FamiliesPath, DefaultFamily)
	{}

	FontFamily::FontFamily(const std::string& name):
		FontFamily(FamiliesPath, name)
	{}

	FontFamily::FontFamily(const std::string& path, const std::string& name):
		_source(GetFullPath(path, name))
	{}

	FontFamily::FontFamily(const FontFamily& other):
		_source(other._source)
	{}

	FontFamily::FontFamily(FontFamily&& other) noexcept:
		_source(std::move(other._source))
	{}

	const std::string& FontFamily::GetSource() const
	{
		return _source;
	}

	FontFamily& FontFamily::operator=(const FontFamily& other)
	{
		_source = other._source;
		return *this;
	}

	FontFamily& FontFamily::operator=(FontFamily&& other) noexcept
	{
		_source = std::move(other._source);
		return *this;
	}

	FontImpl::FontImpl(const FontFamily& fontFamily, float size):
		_font(TTF_OpenFont(fontFamily.GetSource().data(), size))
	{
		if(_font == nullptr)
		{
			SDL_Log("Unable to create a font: %s", SDL_GetError());
		}
	}

	FontImpl::FontImpl(FontImpl&& other) noexcept:
		_font(std::exchange(other._font, nullptr))
	{}

	FontImpl::~FontImpl()
	{
		if(_font)
		{
			TTF_CloseFont(_font);
		}
	}

	void FontImpl::SetSize(float size)
	{
		TTF_SetFontSize(_font, size);
	}

	void FontImpl::SetStyle(FontStyle fontStyle)
	{
		TTF_SetFontStyle(_font, static_cast<unsigned long>(fontStyle));
	}

	void FontImpl::SetOutline(int outline)
	{
		TTF_SetFontOutline(_font, outline);
	}

	void FontImpl::SetFlowDirection(FlowDirection flowDirection)
	{
		auto ttfDirection = static_cast<TTF_Direction>(static_cast<int>(flowDirection) + 4);
		TTF_SetFontDirection(_font, ttfDirection);
	}

	void FontImpl::SetTextAligment(TextAlignment textAlignment)
	{
		TTF_SetFontWrapAlignment(_font, TTF_HorizontalAlignment(textAlignment));
	}

	FontImpl& FontImpl::operator=(FontImpl&& other) noexcept
	{
		_font = std::exchange(other._font, _font);
		return *this;
	}
}
