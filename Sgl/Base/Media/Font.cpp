#include "Font.h"
#include "../Logging.h"
#include "../StringPool.h"

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

	static StringPool FontsPool;

	static inline std::string NormalizePath(const fs::path& base, const std::string& name)
	{
		return (base / name).lexically_normal().string();
	}

	static uint32_t GetDefaultFontPath()
	{
		static uint32_t handle = FontsPool.Create(NormalizePath(FontsPath, DefaultFontName.data()));
		return handle;
	}

	FontFamily::FontFamily(DefaultTag):
		_handle(GetDefaultFontPath()),
		_nameLength(DefaultFontName.length())
	{}

	FontFamily::FontFamily(const std::string& familyName):
		FontFamily(FontsPath, familyName)
	{}

	FontFamily::FontFamily(const std::filesystem::path& basePath, const std::string& familyName):
		_handle(FontsPool.Create(NormalizePath(basePath, familyName))),
		_nameLength(familyName.length())
	{}

	FontFamily::FontFamily(const FontFamily& other):
		_handle(other._handle),
		_nameLength(other._nameLength)
	{}

	FontFamily::FontFamily(FontFamily&& other) noexcept:
		_handle(std::move(other._handle)),
		_nameLength(other._nameLength)
	{}

	std::string_view FontFamily::GetSource() const
	{
		return FontsPool.Get(_handle);
	}

	std::string_view FontFamily::GetName() const
	{
		auto path = FontsPool.Get(_handle);
		return path.substr(path.length() - _nameLength);
	}

	FontFamily& FontFamily::operator=(FontFamily other)
	{
		_handle = other._handle;
		return *this;
	}

	FontFamily& FontFamily::operator=(FontFamily&& other) noexcept
	{
		_handle = other._handle;
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
