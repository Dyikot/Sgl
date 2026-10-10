#include "TextBlock.h"
#include "../Layout/LayoutHelper.h"

#include <SDL3_ttf/SDL_ttf.h>

namespace Sgl::UIElements
{
	static constexpr uint32_t FontFamilyFlag	= 0x01;
	static constexpr uint32_t FontSizeFlag		= 0x02;
	static constexpr uint32_t FontStyleFlag		= 0x04;
	static constexpr uint32_t FontOutlineFlag	= 0x08;
	static constexpr uint32_t FlowDirectionFlag	= 0x10;
	static constexpr uint32_t TextAlignmentFlag	= 0x20;

	TextBlock::TextBlock()
	{
		SetName("TextBlock");
	}

	void TextBlock::SetText(const std::string& value, ValueSource source)
	{
		SetProperty(TextProperty, _text, value, _textSource, source);
	}

	void TextBlock::SetFontSize(float value, ValueSource source)
	{
		SetProperty(FontSizeProperty, _fontSize, value, _fontSizeSource, source);
	}

	void TextBlock::SetFontOutline(int value, ValueSource source)
	{
		SetProperty(FontOutlineProperty, _outline, value, _outlineSource, source);
	}

	void TextBlock::SetFontFamily(FontFamily value, ValueSource source)
	{
		SetProperty(FontFamilyProperty, _fontFamily, value, _fontFamilySource, source);
	}

	void TextBlock::SetFlowDirection(FlowDirection value, ValueSource source)
	{
		SetProperty(FlowDirectionProperty, _flowDirection, value, _flowDirectionSource, source);
	}

	void TextBlock::SetFontStyle(FontStyle value, ValueSource source)
	{
		SetProperty(FontStyleProperty, _fontStyle, value, _fontStyleSource, source);
	}

	void TextBlock::SetForeground(Color value, ValueSource source)
	{
		SetProperty(ForegroundProperty, _foreground, value, _foregroundSource, source);
	}

	Color TextBlock::GetForeground() const
	{
		return GetProperty(ForegroundProperty, _foreground);
	}

	void TextBlock::SetTextWrapping(TextWrapping value, ValueSource source)
	{
		SetProperty(TextWrappingProperty, _textWrapping, value, _textWrappingSource, source);
	}

	void TextBlock::SetTextAlignment(TextAlignment value, ValueSource source)
	{
		SetProperty(TextAlignmentProperty, _textAlignment, value, _textAlignmentSource, source);
	}

	void TextBlock::SetPadding(Thickness value, ValueSource source)
	{
		SetProperty(PaddingProperty, _padding, value, _paddingSource, source);
	}

	void TextBlock::OnRender(RenderContext& context)
	{
		if(auto& textTexture = GetTextTexture(context.GetRenderer()))
		{
			context.DrawTexture(textTexture, &_textBounds, nullptr);
		}
	}

	void TextBlock::OnPropertyChanged(PropertyBase& property)
	{
		UIElement::OnPropertyChanged(property);

		if(property == TextProperty)
		{
			InvalidateTextTexture();
			InvalidateMeasure();
		}
		else if(property == ForegroundProperty)
		{
			InvalidateTextTexture();
			InvalidateRender();
		}
		else if(property == PaddingProperty)
		{
			InvalidateMeasure();
		}
		else if(property == FontSizeProperty)
		{
			InvalidateFont(FontSizeFlag);
			InvalidateTextTexture();
			InvalidateMeasure();
		}
		else if(property == TextAlignmentProperty)
		{
			InvalidateFont(TextAlignmentFlag);
			InvalidateTextTexture();
			InvalidateMeasure();
		}
		else if(property == TextWrappingProperty)
		{
			InvalidateTextTexture();
			InvalidateMeasure();
		}
		else if(property == FontFamilyProperty)
		{
			InvalidateFont(FontFamilyFlag);
			InvalidateTextTexture();
			InvalidateMeasure();
		}
		else if(property == FontStyleProperty)
		{
			InvalidateFont(FontStyleFlag);
			InvalidateTextTexture();
			InvalidateRender();
		}
		else if(property == FontOutlineProperty)
		{
			InvalidateFont(FontOutlineFlag);
			InvalidateTextTexture();
			InvalidateMeasure();
		}
		else if(property == FlowDirectionProperty)
		{
			InvalidateFont(FlowDirectionFlag);
			InvalidateTextTexture();
			InvalidateMeasure();
		}
	}

	void TextBlock::InvalidateTextTexture()
	{
		_textTexture = nullptr;
	}

	void TextBlock::OnDetachedFromLogicalTree()
	{
		UIElement::OnDetachedFromLogicalTree();
		InvalidateTextTexture();
	}

	FSize TextBlock::MeasureContent(FSize availableSize)
	{
		if(_text.empty())
		{
			return FSize();
		}

		if(_fontFlags > 0)
		{
			UpdateFont();
		}

		auto [width, height] = _textWrapping == TextWrapping::NoWrap
			? _font.GetTextSize(_text)
			: _font.GetWrappedTextSize(_text, availableSize.Width);

		FSize size(width, height);
		SetTextBounds(FRect(0, 0, size.Width, size.Height));

		return Expand(size, _padding);
	}

	void TextBlock::ArrangeContent(FRect rect)
	{
		_textBounds.x = rect.x + _padding.Left;
		_textBounds.y = rect.y + _padding.Top;
	}

	void TextBlock::InvalidateFont(uint32_t flag)
	{
		_fontFlags |= flag;
	}

	void TextBlock::UpdateFont()
	{
		if(_fontFlags & FontFamilyFlag)
		{
			_font = Font(_fontFamily, _fontSize);
		}
		else if(_fontFlags & FontSizeFlag)
		{
			_font.SetSize(_fontSize);
		}

		if(_fontFlags & FontStyleFlag)
		{
			_font.SetStyle(_fontStyle);
		}

		if(_fontFlags & FontOutlineFlag)
		{
			_font.SetOutline(_outline);
		}

		if(_fontFlags & FlowDirectionFlag)
		{
			_font.SetFlowDirection(_flowDirection);
		}

		if(_fontFlags & TextAlignmentFlag)
		{
			_font.SetTextAligment(_textAlignment);
		}

		_fontFlags = 0;
	}

	void TextBlock::SetTextBounds(FRect bounds)
	{
		if(_textBounds.w != bounds.w && _textBounds.h != bounds.h)
		{
			_textBounds = bounds;
			InvalidateTextTexture();
		}
	}

	Texture& TextBlock::GetTextTexture(SDL_Renderer* renderer)
	{
		if(!_textTexture && !_text.empty())
		{
			_textTexture = _textWrapping == TextWrapping::NoWrap
				? Texture(renderer, FontQuality::Blended, _font, GetText(), GetForeground())
				: Texture(renderer, FontQuality::Blended, _font, GetText(), _textBounds.w, GetForeground());
		}

		return _textTexture;
	}
}