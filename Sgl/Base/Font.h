#pragma once

#include <string>
#include <bitset>
#include <memory>
#include <SDL/SDL_ttf.h>
#include "Color.h"

namespace Sgl
{
	class FontFamily
	{
	private:
		std::string _source;
	public:
		FontFamily();
		explicit FontFamily(const std::string& name);
		FontFamily(const std::string& path, const std::string& name);
		FontFamily(const FontFamily& other);
		FontFamily(FontFamily&& other) noexcept;

		const std::string& Source() const;

		FontFamily& operator=(const FontFamily& other);
		FontFamily& operator=(FontFamily&& other) noexcept;
	};

	class FontStyle
	{
	private:
		std::bitset<4> _style;
	public:
		FontStyle() = default;

		void SetItalic(bool value = true);
		bool IsItalic() const;

		void SetBold(bool value = true);
		bool IsBold() const;

		void SetUnderline(bool value = true);
		bool HasUnderline() const;

		void SetStrikethrough(bool value = true);
		bool HasStrikethrough() const;

		bool IsNormal() const;
		int ToTTF_Style() const;
	};

	class Font
	{
	public:
		bool Antialiasing;
		Color Color;
	private:
		size_t _size;
		TTF_Font* _font;
		FontStyle _style;
		FontFamily _fontFamily;
	public:
		Font(FontFamily fontFamily, size_t size);
		Font(const Font&) = delete;
		Font(Font&& other) noexcept;
		~Font();

		void SetSize(size_t value);
		size_t GetSize() const;		
		
		void SetStyle(FontStyle value);
		FontStyle GetStyle() const;

		Font& operator=(const Font&) = delete;
		Font& operator=(Font&& other) noexcept;
		operator TTF_Font* () const { return _font; }
	};
}