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
		static constexpr size_t FontFamilyFlag		= 0;
		static constexpr size_t FontSizeFlag		= 1;
		static constexpr size_t FontStyleFlag		= 2;
		static constexpr size_t FontOutlineFlag		= 3;
		static constexpr size_t FlowDirectionFlag	= 4;
		static constexpr size_t TextAlignmentFlag	= 5;

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

		ValueSource _textSource {};
		ValueSource _fontSizeSource {};
		ValueSource _outlineSource {};
		ValueSource _fontFamilySource {};
		ValueSource _flowDirectionSource {};
		ValueSource _fontStyleSource {};
		ValueSource _foregroundSource {};
		ValueSource _textWrappingSource {};
		ValueSource _textAlignmentSource {};
		ValueSource _paddingSource {};

		FRect _textTextureBounds {};
		FontImpl _fontImpl;
		Texture _textTexture;
		bool _isTextTextureValid = false;
		std::bitset<6> _fontFlags = 1;
	public:
		TextBlock();
		TextBlock(const TextBlock& other);
		TextBlock(TextBlock&& other) noexcept;
		~TextBlock() = default;

		void SetText(const std::string& value);
		void SetText(const std::string& value, ValueSource source);
		const std::string& GetText() const { return _text; }

		void SetFontSize(float value);
		void SetFontSize(float value, ValueSource source);
		float GetFontSize() const { return _fontSize; }

		void SetFontOutline(int value);
		void SetFontOutline(int value, ValueSource source);
		int GetFontOutline() const { return _outline; }

		void SetFontFamily(const FontFamily& value);
		void SetFontFamily(const FontFamily& value, ValueSource source);
		const FontFamily& GetFontFamily() const { return _fontFamily; }

		void SetFlowDirection(FlowDirection value);
		void SetFlowDirection(FlowDirection value, ValueSource source);
		FlowDirection GetFlowDirection() const { return _flowDirection; }

		void SetFontStyle(FontStyle value);
		void SetFontStyle(FontStyle value, ValueSource source);
		FontStyle GetFontStyle() const { return _fontStyle; }

		void SetForeground(Color value);
		void SetForeground(Color value, ValueSource source);
		Color GetForeground() const { return _foreground; }

		void SetTextWrapping(TextWrapping value);
		void SetTextWrapping(TextWrapping value, ValueSource source);
		TextWrapping GetTextWrapping() const { return _textWrapping; }

		void SetTextAlignment(TextAlignment value);
		void SetTextAlignment(TextAlignment value, ValueSource source);
		TextAlignment GetTextAlignment() const { return _textAlignment; }

		void SetPadding(Thickness value);
		void SetPadding(Thickness value, ValueSource source);
		Thickness GetPadding() const { return _padding; }

		void Render(RenderContext context) final;
	protected:
		void InvalidateTextTexture();
		FSize MeasureContent(FSize avaliableSize) override;
		void ArrangeContent(FRect rect) override;
	private:
		void InvalidateFont(size_t flag);
		void UpdateFont();
		void CreateTextTexture();
	public:
		static inline StyleableProperty TextProperty { &SetText, &GetText };
		static inline StyleableProperty FontSizeProperty { &SetFontSize, &GetFontSize };
		static inline StyleableProperty FontOutlineProperty { &SetFontOutline, &GetFontOutline };
		static inline StyleableProperty FontFamilyProperty { &SetFontFamily, &GetFontFamily };
		static inline StyleableProperty FlowDirectionProperty { &SetFlowDirection, &GetFlowDirection };
		static inline StyleableProperty FontStyleProperty { &SetFontStyle, &GetFontStyle };
		static inline StyleableProperty ForegroundProperty { &SetForeground, &GetForeground };
		static inline StyleableProperty TextWrappingProperty { &SetTextWrapping, &GetTextWrapping };
		static inline StyleableProperty TextAlignmentProperty { &SetTextAlignment, &GetTextAlignment };
		static inline StyleableProperty PaddingProperty { &SetPadding, &GetPadding };
	};
}