#pragma once

#include "../Base/Media/Font.h"
#include "../UIElement/UIElement.h"

namespace Sgl::UIElements
{
	inline constexpr float DefaultFontSize = 14;

	class TextBlock : public UIElement
	{
	public:
		TextBlock();
		TextBlock(const TextBlock& other);
		TextBlock(TextBlock&& other) noexcept;		

		void SetText(const std::string& value, ValueSource source = ValueSource::Local);
		const std::string& GetText() const { return _text; }

		void SetFontSize(float value, ValueSource source = ValueSource::Local);
		float GetFontSize() const { return _fontSize; }

		void SetFontOutline(int value, ValueSource source = ValueSource::Local);
		int GetFontOutline() const { return _outline; }

		void SetFontFamily(const FontFamily& value, ValueSource source = ValueSource::Local);
		const FontFamily& GetFontFamily() const { return _fontFamily; }

		void SetFlowDirection(FlowDirection value, ValueSource source = ValueSource::Local);
		FlowDirection GetFlowDirection() const { return _flowDirection; }

		void SetFontStyle(FontStyle value, ValueSource source = ValueSource::Local);
		FontStyle GetFontStyle() const { return _fontStyle; }

		void SetForeground(Color value, ValueSource source = ValueSource::Local);
		Color GetForeground() const { return _foreground; }

		void SetTextWrapping(TextWrapping value, ValueSource source = ValueSource::Local);
		TextWrapping GetTextWrapping() const { return _textWrapping; }

		void SetTextAlignment(TextAlignment value, ValueSource source = ValueSource::Local);
		TextAlignment GetTextAlignment() const { return _textAlignment; }

		void SetPadding(Thickness value, ValueSource source = ValueSource::Local);
		Thickness GetPadding() const { return _padding; }

		void Render(RenderContext context) final;

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
	protected:
		void InvalidateTextTexture();
		FSize MeasureContent(FSize avaliableSize) override;
		void ArrangeContent(FRect rect) override;
	private:
		void InvalidateFont(size_t flag);
		void UpdateFont();
		void CreateTextTexture();
	private:
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
		uint32_t _fontFlags = 1;
	};
}