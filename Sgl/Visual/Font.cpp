#include "Font.h"
#include <unordered_map>
#include "../Tools/Log.h"

namespace Sgl
{
	static std::unordered_map<std::string, std::string> FontFamilies =
	{
		{ "Segoe UI", "/" },
	};

	FontFamily::FontFamily(const std::string& name)
	{
		auto it = FontFamilies.find(name);
		if(it == FontFamilies.end())
		{
			auto message = std::format("'{}' font family does not exist!", name);
			throw std::exception(message.c_str());
		}

		 _name = &it->first;
	}

	FontFamily::FontFamily(std::string name, std::string path)
	{
		auto [it, _] = FontFamilies.try_emplace(std::move(name), std::move(path));
		_name = &it->first;
	}

	FontFamily::FontFamily(const FontFamily& other):
		_name(other._name)
	{}

	FontFamily::FontFamily(FontFamily&& other) noexcept:
		_name(std::exchange(other._name, nullptr))
	{}

	const std::string& FontFamily::Name() const
	{
		return *_name;
	}

	const std::string& FontFamily::Path() const
	{
		return FontFamilies[*_name];
	}

	FontFamily& FontFamily::operator=(const FontFamily& other)
	{
		_name = other._name;
		return *this;
	}

	FontFamily& FontFamily::operator=(FontFamily&& other) noexcept
	{
		_name = std::exchange(other._name, nullptr);
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
	
	std::shared_ptr<Font> Font::Default()
	{
		static auto font = std::make_shared<Font>(FontFamily("Segoe UI"), 14);
		return font;
	}

	Font::Font(FontFamily fontFamily, size_t size):
		Antialiasing(true),
		Color(Colors::Black),
		_style(),
		_size(size),
		_font(TTF_OpenFont(fontFamily.Name().data(), size)),
		_fontFamily(fontFamily)
	{
		Log::PrintSDLErrorIf(_font == nullptr);
	}

	Font::Font(Font&& other) noexcept:
		Antialiasing(other.Antialiasing),
		Color(other.Color),
		_style(other._style),
		_size(other._size),
		_font(std::exchange(other._font, nullptr)),
		_fontFamily(other._fontFamily)
	{}

	Font::~Font()
	{
		if(_font)
		{
			TTF_CloseFont(_font);
		}
	}

	void Font::SetSize(size_t value)
	{
		TTF_SetFontSize(_font, value);
		_size = value;
	}

	size_t Font::GetSize() const
	{
		return _size;
	}

	void Font::SetStyle(FontStyle value)
	{
		_style = value;
		TTF_SetFontStyle(_font, _style.ToTTF_Style());
	}

	FontStyle Font::GetStyle() const
	{
		return _style;
	}

	Font& Font::operator=(Font&& other) noexcept
	{
		Antialiasing = other.Antialiasing;
		Color = other.Color;
		_style = other._style;
		_size = other._size;
		_font = std::exchange(other._font, nullptr);
		_fontFamily = other._fontFamily;
		return *this;
	}
}
