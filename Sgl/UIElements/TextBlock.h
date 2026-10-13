#pragma once

#include "../Base/Media/Font.h"
#include "UIElement.h"

namespace Sgl::UIElements
{
	class TextBlock : public UIElement
	{
	public:
		TextBlock();

		void SetText(const std::string& value);
		const std::string& GetText() const { return _text; }

		void SetFontSize(float value);
		float GetFontSize() const { return _fontSize; }

		void SetFontOutline(int value);
		int GetFontOutline() const { return _outline; }

		void SetFontFamily(FontFamily value);
		FontFamily GetFontFamily() const { return _fontFamily; }

		void SetFlowDirection(FlowDirection value);
		FlowDirection GetFlowDirection() const { return _flowDirection; }

		void SetFontStyle(FontStyle value);
		FontStyle GetFontStyle() const { return _fontStyle; }

		void SetForeground(Color value);
		Color GetForeground() const;

		void SetTextWrapping(TextWrapping value);
		TextWrapping GetTextWrapping() const { return _textWrapping; }

		void SetTextAlignment(TextAlignment value);
		TextAlignment GetTextAlignment() const { return _textAlignment; }

		void SetPadding(Thickness value);
		Thickness GetPadding() const { return _padding; }

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
		void OnRender(RenderContext& context) override;
		void OnPropertyChanged(PropertyBase& property) override;
		void InvalidateTextTexture();
		void OnDetachedFromLogicalTree() override;
		FSize MeasureContent(FSize availableSize) override;
		void ArrangeContent(FRect rect) override;
	private:
		void InvalidateFont(uint32_t flag);
		void UpdateFont();
		void SetTextBounds(FRect bounds);
		Texture& GetTextTexture(SDL_Renderer* renderer);
	private:
		std::string _text;
		float _fontSize = 16;
		int _outline = 0;
		FontFamily _fontFamily = FontFamily::GetDefault();
		FlowDirection _flowDirection = FlowDirection::BottomToTop;
		FontStyle _fontStyle = FontStyle::Normal;
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

		FRect _textBounds {};
		Font _font;
		Texture _textTexture;
		uint32_t _fontFlags = 1;
	};
}