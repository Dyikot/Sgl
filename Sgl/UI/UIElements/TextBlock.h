#pragma once

#include "../UIElement.h"
#include "../../Base/Media/Font.h"
#include "../../Base/Media/Text.h"

namespace Sgl::UIElements
{
	class TextBlock: public UIElement
	{
	public:
		static constexpr size_t DefaultFontSize = 14;
	private:
		static constexpr unsigned long FontFamilyFlag = 1;
		static constexpr unsigned long FontSizeFlag = 2;
		static constexpr unsigned long FontStyleFlag = 4;

		std::string _text;
		size_t _fontSize = DefaultFontSize;
		FontFamily _fontFamily = FontFamily::Default;
		FontStyle _fontStyle;
		Color _foreground = Colors::Black;
		TextWrapping _textWrapping = TextWrapping::NoWrap;
		TextAlignment _textAlignment = TextAlignment::Left;
		Thickness _padding;

		FRect _textTextureBounds {};
		FontImpl _fontImpl;
		Unique<Texture> _textTexture;
		bool _isTextTextureValid = false;
		std::bitset<3> _fontValidationFlags = FontFamilyFlag;
	public:
		TextBlock() = default;
		TextBlock(const TextBlock& other);
		TextBlock(TextBlock&& other) noexcept;
		~TextBlock() = default;

		void SetText(const std::string& value)
		{
			SetProperty(TextProperty, _text, value);
			InvalidateTextTexture();
			InvalidateMeasure();
		}

		const std::string& GetText() const
		{
			return _text;
		}

		void SetFontSize(size_t value)
		{
			SetProperty(FontSizeProperty, _fontSize, value);
			InvalidateFont(FontSizeFlag);
			InvalidateTextTexture();
			InvalidateMeasure();
		}

		size_t GetFontSize() const
		{
			return _fontSize;
		}

		void SetFontFamily(const FontFamily& value)
		{
			SetProperty(FontFamilyProperty, _fontFamily, value);
			InvalidateFont(FontFamilyFlag);
			InvalidateTextTexture();
			InvalidateMeasure();
		}

		const FontFamily& GetFontFamily() const
		{
			return _fontFamily;
		}

		void SetFontStyle(FontStyle value)
		{
			SetProperty(FontStyleProperty, _fontStyle, value);
			InvalidateFont(FontStyleFlag);
			InvalidateTextTexture();
		}

		FontStyle GetFontStyle() const
		{
			return _fontStyle;
		}

		void SetForeground(Color value)
		{
			SetProperty(ForegroundProperty, _foreground, value);
			InvalidateTextTexture();
		}

		Color GetForeground() const
		{
			return _foreground;
		}

		void SetTextWrapping(TextWrapping value)
		{
			SetProperty(TextWrappingProperty, _textWrapping, value);
			InvalidateTextTexture();
			InvalidateMeasure();
		}

		TextWrapping GetTextWrapping() const
		{
			return _textWrapping;
		}

		void SetTextAlignment(TextAlignment value)
		{
			SetProperty(TextAlignmentProperty, _textAlignment, value);
			InvalidateTextTexture();
			InvalidateMeasure();
		}

		TextAlignment GetTextAlignment() const
		{
			return _textAlignment;
		}

		void SetPadding(Thickness value)
		{
			SetProperty(PaddingProperty, _padding, value);
			InvalidateMeasure();
		}

		Thickness GetPadding() const
		{
			return _padding;
		}

		void Render(RenderContext context) override;
	protected:
		void InvalidateTextTexture() { _isTextTextureValid = false; }
		FSize MeasureContent(FSize avaliableSize) override;
		void ArrangeContent(FRect rect) override;
	private:
		void InvalidateFont(size_t flag) { _fontValidationFlags |= flag; }
		void CreateOrUpdateFont();
		void CreateTextTextureIfInvalid(float maxLineWidth);
	public:
		static inline BindableProperty<TextBlock, const std::string&> TextProperty =
			BindableProperty<TextBlock, const std::string&>(&SetText);

		static inline BindableProperty<TextBlock, size_t> FontSizeProperty =
			BindableProperty<TextBlock, size_t>(&SetFontSize);

		static inline BindableProperty<TextBlock, const FontFamily&> FontFamilyProperty =
			BindableProperty<TextBlock,const FontFamily&>(&SetFontFamily);

		static inline BindableProperty<TextBlock, FontStyle> FontStyleProperty =
			BindableProperty<TextBlock, FontStyle>(&SetFontStyle);

		static inline BindableProperty<TextBlock, Color> ForegroundProperty =
			BindableProperty<TextBlock, Color>(&SetForeground);

		static inline BindableProperty<TextBlock, TextWrapping> TextWrappingProperty =
			BindableProperty<TextBlock, TextWrapping>(&SetTextWrapping);

		static inline BindableProperty<TextBlock, TextAlignment> TextAlignmentProperty =
			BindableProperty<TextBlock, TextAlignment>(&SetTextAlignment);

		static inline BindableProperty<TextBlock, Thickness> PaddingProperty =
			BindableProperty<TextBlock, Thickness>(&SetPadding);
	};
}

namespace Sgl
{
	struct StringDataTemplate
	{
		Shared<UIElement> operator()(const Any& data) const
		{
			const auto& text = data.As<std::string>();
			auto textBlock = NewShared<UIElements::TextBlock>();
			textBlock->SetText(text);
			return textBlock;
		}
	};
}