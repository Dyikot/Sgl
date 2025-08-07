#include "Font.h"
#include <unordered_map>
#include <filesystem>
#include "../Log.h"

namespace Sgl
{
	static constexpr auto FalimiesPath = "C:/Windows/Fonts/";
	static constexpr auto DefaultFamily = "Segoe UI";

	static std::string GetFullPath(const std::string& path, const std::string& name)
	{
		auto basePath = std::filesystem::path(path) / name;		
		return basePath.string();
	}

	FontFamily::FontFamily(DefaultFontFamilty):
		_source(FalimiesPath, DefaultFamily)
	{}

	FontFamily::FontFamily(const std::string& name):
		FontFamily(FalimiesPath, name)
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

	const std::string& FontFamily::Source() const
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

	void FontStyle::SetItalic(bool value)
	{
		_style.set(TTF_STYLE_ITALIC, value);
	}

	bool FontStyle::IsItalic() const
	{
		return _style[TTF_STYLE_ITALIC];
	}

	void FontStyle::SetBold(bool value)
	{
		_style.set(TTF_STYLE_BOLD, value);
	}

	bool FontStyle::IsBold() const
	{
		return _style[TTF_STYLE_BOLD];
	}

	void FontStyle::SetUnderline(bool value)
	{
		_style.set(TTF_STYLE_UNDERLINE, value);
	}

	bool FontStyle::HasUnderline() const
	{
		return _style[TTF_STYLE_UNDERLINE];
	}

	void FontStyle::SetStrikethrough(bool value)
	{
		_style.set(TTF_STYLE_STRIKETHROUGH, value);
	}

	bool FontStyle::HasStrikethrough() const
	{
		return _style[TTF_STYLE_STRIKETHROUGH];
	}

	bool FontStyle::IsNormal() const
	{
		return _style.none();
	}

	int FontStyle::ToTTF_Style() const
	{
		return _style.to_ulong();
	}

	FontImpl::FontImpl(const FontFamily& fontFamily, size_t size):
		_font(TTF_OpenFont(fontFamily.Source().data(), size))
	{
		Log::PrintSDLErrorIf(_font == nullptr);
	}

	FontImpl::~FontImpl()
	{
		if(_font)
		{
			TTF_CloseFont(_font);
		}
	}
}
