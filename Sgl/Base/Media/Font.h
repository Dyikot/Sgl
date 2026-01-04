#pragma once

#include <string>
#include <bitset>
#include <memory>
#include "Color.h"

struct TTF_Font;

namespace Sgl
{
	class FontFamily final
	{	
	private:
		struct FontFamilyImpl;
		struct DefaultTag {};
	public:
		static constexpr DefaultTag Default;
	private:
		FontFamilyImpl* _impl = nullptr;
	public:
		FontFamily(DefaultTag);
		explicit FontFamily(const std::string& name);
		FontFamily(const std::string& path, const std::string& name);
		FontFamily(const FontFamily& other);
		FontFamily(FontFamily&& other) noexcept;
		~FontFamily();

		const std::string& GetSource() const;

		FontFamily& operator=(const FontFamily& other);
		FontFamily& operator=(FontFamily&& other) noexcept;
		friend bool operator==(const FontFamily&, const FontFamily&) = default;
	private:
		void CopyFrom(const FontFamily& other);
		void Release();
	};

	enum class FontStyles
	{
		Normal = 0,
		Bold = 1,
		Italic = 2,
		Underline = 4,
		Strikethrough = 8
	};

	class FontStyle
	{
	private:
		std::bitset<4> _styles;
	public:
		constexpr FontStyle() = default;
		template<std::same_as<FontStyles>... TStyles>
		constexpr FontStyle(TStyles... styles):
			_styles((static_cast<unsigned long>(styles) | ...))
		{}
		constexpr FontStyle(const FontStyle&) = default;
		constexpr FontStyle(FontStyle&&) = default;

		constexpr bool IsItalic() const { return _styles[0]; }
		constexpr bool IsBold() const { return _styles[1]; }
		constexpr bool HasUnderline() const { return _styles[2]; }
		constexpr bool HasStrikethrough() const { return _styles[3]; }

		constexpr FontStyle& operator=(const FontStyle&) = default;
		constexpr FontStyle& operator=(FontStyle&&) = default;

		friend bool operator==(const FontStyle&, const FontStyle&) = default;
		operator unsigned long() const noexcept { return _styles.to_ulong(); }
	};

	enum class FlowDirection
	{
		LeftToRight,
		RightToLeft,
		BottomToTop,
		TopToBottom
	};

	enum class TextWrapping
	{
		NoWrap, Wrap
	};

	enum class TextAlignment
	{
		Left, Center, Right
	};

	enum class FontQuality
	{
		Blended, Solid, Shaded, LCD
	};

	class FontImpl final
	{
	private:
		TTF_Font* _font = nullptr;
	public:
		FontImpl() = default;
		FontImpl(const FontFamily& fontFamily, float size);
		FontImpl(const FontImpl&) = delete;
		FontImpl(FontImpl&& other) noexcept;
		~FontImpl();

		void SetSize(float size);
		void SetStyle(FontStyle fontStyle);
		void SetOutline(int outline);
		void SetFlowDirection(FlowDirection flowDirection);
		void SetTextAligment(TextAlignment textAlignment);

		FontImpl& operator=(const FontImpl&) = delete;
		FontImpl& operator=(FontImpl&& other) noexcept;
		operator TTF_Font* () const { return _font; }
	};		
}