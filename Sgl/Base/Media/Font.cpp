#include "Font.h"
#include <unordered_map>
#include <filesystem>
#include "../Log.h"

namespace Sgl
{
	static constexpr auto FalimiesPath = "C:/Windows/Fonts/";
	static constexpr auto DefaultFamily = "segoeui.ttf";

	static std::string GetFullPath(const std::string& path, const std::string& name)
	{
		auto basePath = std::filesystem::path(path) / name;		
		return basePath.string();
	}

	FontFamily::FontFamily(DefaultFontFamilty):
		FontFamily(FalimiesPath, DefaultFamily)
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

	FontImpl::FontImpl(const FontFamily& fontFamily, size_t size):
		_font(TTF_OpenFont(fontFamily.Source().data(), size))
	{
		Log::PrintSDLErrorIf(_font == nullptr);
	}

	FontImpl::FontImpl(FontImpl&& other) noexcept:
		_font(std::exchange(other._font, nullptr))
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

	FontImpl& FontImpl::operator=(FontImpl&& other) noexcept
	{
		_font = std::exchange(other._font, nullptr);
		return *this;
	}
}
