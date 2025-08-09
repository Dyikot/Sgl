#pragma once

#include "../UIElement.h"
#include "../../Base/Media/Font.h"
#include "../../Base/Media/Text.h"

namespace Sgl::UIElements
{
	class TextBlock: public UIElement
	{
	private:
		static constexpr unsigned long _fontFamilyFlag = 1;
		static constexpr unsigned long _fontSizeFlag = 2;
		static constexpr unsigned long _fontStyleFlag = 4;

		std::string _text;
		size_t _fontSize;
		FontFamily _fontFamily;
		FontStyle _fontStyle;
		Color _foreground;
		TextWrapping _textWrapping;
		TextAlignment _textAlignment;
		Thickness _padding;

		FRect _textTextureBounds;
		FontImpl _fontImpl;
		Unique<Texture> _textTexture;
		bool _isTextTextureValid;
		std::bitset<3> _fontValidationFlags;
	public:
		TextBlock();
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
			InvalidateFont(_fontSizeFlag);
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
			InvalidateFont(_fontFamilyFlag);
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
			InvalidateFont(_fontStyleFlag);
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
		static inline BindableProperty<TextBlock, std::string, const std::string&> TextProperty =
			BindableProperty<TextBlock, std::string, const std::string&>(&SetText);

		static inline BindableProperty<TextBlock, size_t> FontSizeProperty =
			BindableProperty<TextBlock, size_t>(&SetFontSize, 14);

		static inline BindableProperty<TextBlock, FontFamily, const FontFamily&> FontFamilyProperty =
			BindableProperty<TextBlock, FontFamily, const FontFamily&>(&SetFontFamily, FontFamily::Default);

		static inline BindableProperty<TextBlock, FontStyle> FontStyleProperty =
			BindableProperty<TextBlock, FontStyle>(&SetFontStyle);

		static inline BindableProperty<TextBlock, Color> ForegroundProperty =
			BindableProperty<TextBlock, Color>(&SetForeground, Colors::Black);

		static inline BindableProperty<TextBlock, TextWrapping> TextWrappingProperty =
			BindableProperty<TextBlock, TextWrapping>(&SetTextWrapping, TextWrapping::NoWrap);

		static inline BindableProperty<TextBlock, TextAlignment> TextAlignmentProperty =
			BindableProperty<TextBlock, TextAlignment>(&SetTextAlignment, TextAlignment::Left);

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