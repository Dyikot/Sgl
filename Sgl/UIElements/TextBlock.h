#pragma once

#include "../Base/Media/Font.h"
#include "../UIElement/UIElement.h"

namespace Sgl::UIElements
{
	class TextBlock : public UIElement
	{
	public:
		static constexpr float DefaultFontSize = 14;
	private:
		static constexpr size_t FontFamilyBit		= 0;
		static constexpr size_t FontSizeBit			= 1;
		static constexpr size_t FontStyleBit		= 2;
		static constexpr size_t FontOutlineBit		= 3;
		static constexpr size_t FlowDirectionBit	= 4;
		static constexpr size_t TextAlignmentBit	= 5;

		std::string _text;
		float _fontSize = DefaultFontSize;
		int _outline = 0;
		FontFamily _fontFamily = FontFamily::Default;
		FlowDirection _flowDirection = FlowDirection::BottomToTop;
		FontStyle _fontStyle;
		Color _foreground = Colors::Black;
		TextWrapping _textWrapping = TextWrapping::NoWrap;
		TextAlignment _textAlignment = TextAlignment::Left;
		Thickness _padding;

		FRect _textTextureBounds {};
		FontImpl _fontImpl;
		Texture _textTexture;
		bool _isTextTextureValid = false;
		std::bitset<6> _fontValidationBits = 1;
	public:
		TextBlock();
		TextBlock(const TextBlock& other);
		TextBlock(TextBlock&& other) noexcept;
		~TextBlock() = default;

		void SetText(const std::string& value);
		const std::string& GetText() const { return _text; }

		void SetFontSize(float value); 
		float GetFontSize() const { return _fontSize; }

		void SetFontOutline(int value);
		int GetFontOutline() const { return _outline; }

		void SetFontFamily(const FontFamily& value);
		const FontFamily& GetFontFamily() const { return _fontFamily; }

		void SetFlowDirection(FlowDirection value);
		FlowDirection GetFlowDirection() const { return _flowDirection; }

		void SetFontStyle(FontStyle value);
		FontStyle GetFontStyle() const { return _fontStyle; }

		void SetForeground(Color value);
		Color GetForeground() const { return _foreground; }

		void SetTextWrapping(TextWrapping value); 
		TextWrapping GetTextWrapping() const { return _textWrapping; }

		void SetTextAlignment(TextAlignment value);
		TextAlignment GetTextAlignment() const { return _textAlignment; }

		void SetPadding(Thickness value);
		Thickness GetPadding() const { return _padding; }

		void Render(RenderContext context) final;
	protected:
		void InvalidateTextTexture();
		FSize MeasureContent(FSize avaliableSize) override;
		void ArrangeContent(FRect rect) override;
	private:
		void InvalidateFont(size_t bit);
		void UpdateFont();
		void CreateTextTexture();
	public:
		static inline SglProperty TextProperty { &SetText, &GetText };
		static inline SglProperty FontSizeProperty { &SetFontSize, &GetFontSize };
		static inline SglProperty FontOutlineProperty { &SetFontOutline, &GetFontOutline };
		static inline SglProperty FontFamilyProperty { &SetFontFamily, &GetFontFamily };
		static inline SglProperty FlowDirectionProperty { &SetFlowDirection, &GetFlowDirection };
		static inline SglProperty FontStyleProperty { &SetFontStyle, &GetFontStyle };
		static inline SglProperty ForegroundProperty { &SetForeground, &GetForeground };
		static inline SglProperty TextWrappingProperty { &SetTextWrapping, &GetTextWrapping };
		static inline SglProperty TextAlignmentProperty { &SetTextAlignment, &GetTextAlignment };
		static inline SglProperty PaddingProperty { &SetPadding, &GetPadding };
	};
}