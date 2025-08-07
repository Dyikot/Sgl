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
		struct DefaultFontFamilty {};
	public:
		static constexpr DefaultFontFamilty Default;
	private:
		std::string _source;
	public:
		FontFamily(DefaultFontFamilty);
		explicit FontFamily(const std::string& name);
		FontFamily(const std::string& path, const std::string& name);
		FontFamily(const FontFamily& other);
		FontFamily(FontFamily&& other) noexcept;

		const std::string& Source() const;

		FontFamily& operator=(const FontFamily& other);
		FontFamily& operator=(FontFamily&& other) noexcept;
	};

	enum class FontWeight
	{
		Normal, Bold
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

	class FontImpl
	{
	private:
		TTF_Font* _font;
	public:
		FontImpl(const FontFamily& fontFamily, size_t size);
		FontImpl(const FontImpl&) = delete;
		FontImpl(FontImpl&&) = default;
		~FontImpl();

		void SetSize(size_t value)
		{
			TTF_SetFontSize(_font, value);
		}

		void SetStyle(FontStyle value)
		{
			TTF_SetFontStyle(_font, value.ToTTF_Style());
		}

		FontImpl& operator=(const FontImpl&) = delete;
		FontImpl& operator=(FontImpl&&) = default;
		operator TTF_Font* () const { return _font; }
	};
}