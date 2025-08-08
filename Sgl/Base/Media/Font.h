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
		friend bool operator==(const FontFamily&, const FontFamily&) = default;
	};

	class FontStyle
	{
	public:
		static constexpr unsigned long Bold = 1;
		static constexpr unsigned long Italic = 2;
		static constexpr unsigned long Underline = 4;
		static constexpr unsigned long Strikethrough = 8;
	private:
		std::bitset<4> _flags;
	public:
		constexpr FontStyle() = default;
		constexpr FontStyle(unsigned long flags): _flags(flags) {}
		constexpr FontStyle(const FontStyle&) = default;
		constexpr FontStyle(FontStyle&&) = default;

		constexpr bool IsItalic() const { return _flags[0]; }
		constexpr bool IsBold() const { return _flags[1]; }
		constexpr bool HasUnderline() const { return _flags[2]; }
		constexpr bool HasStrikethrough() const { return _flags[3]; }

		constexpr FontStyle& operator=(const FontStyle&) = default;
		constexpr FontStyle& operator=(FontStyle&&) = default;

		friend bool operator==(const FontStyle&, const FontStyle&) = default;
		operator unsigned long() const noexcept { return _flags.to_ulong(); }
	};

	class FontImpl
	{
	private:
		TTF_Font* _font = nullptr;
	public:
		FontImpl() = default;
		FontImpl(const FontFamily& fontFamily, size_t size);
		FontImpl(const FontImpl&) = delete;
		FontImpl(FontImpl&& other) noexcept;
		~FontImpl();

		void SetSize(size_t value)
		{
			TTF_SetFontSize(_font, value);
		}

		void SetStyle(FontStyle value)
		{
			TTF_SetFontStyle(_font, static_cast<unsigned long>(value));
		}

		FontImpl& operator=(const FontImpl&) = delete;
		FontImpl& operator=(FontImpl&& other) noexcept;
		operator TTF_Font* () const { return _font; }
	};

	enum class FontRenderType
	{
		Blended, Solid, Shaded, LCD 
	};
}