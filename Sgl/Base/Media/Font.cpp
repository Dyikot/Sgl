#include "Font.h"
#include "../Logging.h"

#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_platform_defines.h>

namespace fs = std::filesystem;

namespace Sgl
{
	class FontFamily::Impl : public RefCounted
	{
	public:
		Impl(const fs::path& base, const std::string& name):
			Name(name)
		{
			if(base.empty())
			{
				throw std::invalid_argument("Font name must not be empty");
			}

			Source = (base / name).lexically_normal();
		}

		std::string Name;
		fs::path Source;
	};

	#ifdef SDL_PLATFORM_WIN32
	static constexpr auto FontsPath = "C:/Windows/Fonts/";
	static constexpr auto FontFileName = "segoeui.ttf";
	#elif SDL_PLATFORM_LINUX
	static constexpr auto FontsPath = "/usr/share/fonts/";
	static constexpr auto FontFileName = "DejaVuSans.ttf";
	#elif SDL_PLATFORM_MACOS
	static constexpr auto FontsPath = "/System/Library/Fonts/";
	static constexpr auto FontFileName = "Helvetica.ttf";
	#endif
	
	FontFamily::FontFamily(const std::string& fontFileName):
		FontFamily(FontsPath, fontFileName)
	{}

	FontFamily::FontFamily(const fs::path& basePath, const std::string& fontFileName):
		_impl(New<FontFamily::Impl>(basePath, fontFileName))
	{}

	FontFamily::FontFamily(const FontFamily& other):
		_impl(other._impl)
	{}

	FontFamily::FontFamily(FontFamily&& other) noexcept:
		_impl(std::move(other._impl))
	{}

	FontFamily::~FontFamily() = default;

	FontFamily FontFamily::GetDefault()
	{
		static FontFamily instance(FontsPath, FontFileName);
		return instance;
	}

	const fs::path& FontFamily::GetSource() const
	{
		return _impl->Source;
	}

	const std::string& FontFamily::GetName() const
	{
		return _impl->Name;
	}

	FontFamily& FontFamily::operator=(const FontFamily& other)
	{
		_impl = other._impl;
		return *this;
	}

	FontFamily& FontFamily::operator=(FontFamily&& other) noexcept
	{
		_impl = std::move(other._impl);
		return *this;
	}

	bool FontFamily::operator==(const FontFamily& other) const
	{
		return _impl == other._impl;
	}

	Font::Font(FontFamily fontFamily, float size)
	{
		auto source = fontFamily.GetSource().u8string();
		_impl = TTF_OpenFont(reinterpret_cast<const char*>(source.c_str()), size);

		if(_impl == nullptr)
		{
			Logging::LogError("Unable to create a font: {}", SDL_GetError());
		}
	}

	Font::Font(Font&& other) noexcept:
		_impl(std::exchange(other._impl, nullptr))
	{}

	Font::~Font()
	{
		if(_impl)
		{
			TTF_CloseFont(_impl);
		}
	}

	void Font::SetSize(float size)
	{
		TTF_SetFontSize(_impl, size);
	}

	void Font::SetStyle(FontStyle fontStyle)
	{
		TTF_SetFontStyle(_impl, static_cast<int>(fontStyle));
	}

	void Font::SetOutline(int outline)
	{
		TTF_SetFontOutline(_impl, outline);
	}

	void Font::SetFlowDirection(FlowDirection flowDirection)
	{
		TTF_Direction direction;

		switch(flowDirection)
		{
			case FlowDirection::LeftToRight: direction = TTF_DIRECTION_LTR; break;
			case FlowDirection::RightToLeft: direction = TTF_DIRECTION_RTL; break;
			case FlowDirection::BottomToTop: direction = TTF_DIRECTION_BTT; break;
			case FlowDirection::TopToBottom: direction = TTF_DIRECTION_TTB; break;
			default:                         direction = TTF_DIRECTION_INVALID; break;
		}

		TTF_SetFontDirection(_impl, direction);
	}

	void Font::SetTextAligment(TextAlignment textAlignment)
	{
		TTF_HorizontalAlignment alignment;

		switch(textAlignment)
		{
			case TextAlignment::Left:   alignment = TTF_HORIZONTAL_ALIGN_LEFT; break;
			case TextAlignment::Center: alignment = TTF_HORIZONTAL_ALIGN_CENTER; break;
			case TextAlignment::Right:  alignment = TTF_HORIZONTAL_ALIGN_RIGHT;  break;
			default:                    alignment = TTF_HORIZONTAL_ALIGN_INVALID; break;
		}

		TTF_SetFontWrapAlignment(_impl, alignment);
	}

	Font& Font::operator=(Font&& other) noexcept
	{
		_impl = std::exchange(other._impl, _impl);
		return *this;
	}
}
