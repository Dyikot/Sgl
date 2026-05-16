#include "Font.h"
#include "../Logging.h"

#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_platform_defines.h>

namespace fs = std::filesystem;

namespace Sgl
{
	#ifdef SDL_PLATFORM_WIN32
	static constexpr auto FontsPath = "C:/Windows/Fonts/";
	#elif SDL_PLATFORM_LINUX
	static constexpr auto FontsPath = "/usr/share/fonts/";
	#elif SDL_PLATFORM_MACOS
	static constexpr auto FontsPath = "/System/Library/Fonts/";
	#endif

	static constexpr std::string_view DefaultFontName = "segoeui.ttf";

	static SourcePath GetDefaultSource()
	{
		static SourcePath defaultFilePath(FontsPath, DefaultFontName);
		return defaultFilePath;
	}

	FontFamily::FontFamily(DefaultTag):
		_fontPath(GetDefaultSource()),
		_nameLength(DefaultFontName.length())
	{}

	FontFamily::FontFamily(const std::string& familyName):
		FontFamily(FontsPath, familyName)
	{}

	FontFamily::FontFamily(const std::filesystem::path& basePath, const std::string& familyName):
		_fontPath(basePath, familyName),
		_nameLength(familyName.length())
	{}

	FontFamily::FontFamily(const FontFamily& other):
		_fontPath(other._fontPath),
		_nameLength(other._nameLength)
	{}

	FontFamily::FontFamily(FontFamily&& other) noexcept:
		_fontPath(std::move(other._fontPath)),
		_nameLength(other._nameLength)
	{}

	std::string_view FontFamily::GetSource() const
	{
		return _fontPath.Path();
	}

	std::string_view FontFamily::GetName() const
	{
		auto path = _fontPath.Path();
		return path.substr(path.length() - _nameLength);
	}

	FontFamily& FontFamily::operator=(FontFamily other)
	{
		_fontPath = other._fontPath;
		return *this;
	}

	FontFamily& FontFamily::operator=(FontFamily&& other) noexcept
	{
		_fontPath = other._fontPath;
		return *this;
	}

	TrueTypeFont::TrueTypeFont(FontFamily fontFamily, float size):
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
		TTF_SetFontStyle(_font, static_cast<int>(fontStyle));
	}

	void TrueTypeFont::SetOutline(int outline)
	{
		TTF_SetFontOutline(_font, outline);
	}

	void TrueTypeFont::SetFlowDirection(FlowDirection flowDirection)
	{
		TTF_SetFontDirection(_font, TTF_Direction(static_cast<int>(flowDirection) + 4));
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
