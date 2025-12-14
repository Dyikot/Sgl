#include "TextBlock.h"
#include <algorithm>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>

namespace Sgl::UIElements
{
	TextBlock::TextBlock()
	{
		
	}

	TextBlock::TextBlock(const TextBlock& other):
		UIElement(other),
		_text(other._text),
		_fontSize(other._fontSize),
		_outline(other._outline),
		_fontFamily(other._fontFamily),
		_flowDirection(other._flowDirection),
		_fontStyle(other._fontStyle),
		_foreground(other._foreground),
		_textWrapping(other._textWrapping),
		_textAlignment(other._textAlignment),
		_padding(other._padding),
		_textTextureBounds(other._textTextureBounds),
		_fontImpl(),
		_textTexture(),
		_isTextTextureValid(),
		_fontValidationBits(FontFamilyBit)
	{}

	TextBlock::TextBlock(TextBlock&& other) noexcept:
		UIElement(std::move(other)),
		_text(std::move(other._text)),
		_fontSize(other._fontSize),
		_outline(other._outline),
		_fontFamily(std::move(other._fontFamily)),
		_flowDirection(other._flowDirection),
		_fontStyle(other._fontStyle),
		_foreground(other._foreground),
		_textWrapping(other._textWrapping),
		_textAlignment(other._textAlignment),
		_padding(other._padding),
		_textTextureBounds(other._textTextureBounds),
		_fontImpl(std::move(other._fontImpl)),
		_textTexture(std::move(other._textTexture)),
		_isTextTextureValid(other._isTextTextureValid),
		_fontValidationBits(other._fontValidationBits)
	{}

	void TextBlock::SetText(const std::string& value)
	{
		if(SetProperty(TextProperty, _text, value))
		{
			InvalidateTextTexture();
			InvalidateMeasure();
		}
	}	

	void TextBlock::SetFontSize(float value)
	{
		if(SetProperty(FontSizeProperty, _fontSize, value))
		{
			InvalidateFont(FontSizeBit);
			InvalidateTextTexture();
			InvalidateMeasure();
		}		
	}

	void TextBlock::SetFontOutline(int value)
	{
		if(SetProperty(FontOutlineProperty, _outline, value))
		{
			InvalidateFont(FontOutlineBit);
			InvalidateTextTexture();
			InvalidateMeasure();
		}		
	}

	void TextBlock::SetFontFamily(const FontFamily& value)
	{
		if(SetProperty(FontFamilyProperty, _fontFamily, value))
		{
			InvalidateFont(FontFamilyBit);
			InvalidateTextTexture();
			InvalidateMeasure();
		}		
	}

	void TextBlock::SetFlowDirection(FlowDirection value)
	{
		if(SetProperty(FlowDirectionProperty, _flowDirection, value))
		{
			InvalidateFont(FlowDirectionBit);
			InvalidateTextTexture();
			InvalidateMeasure();
		}		
	}

	void TextBlock::SetFontStyle(FontStyle value)
	{
		if(SetProperty(FontStyleProperty, _fontStyle, value))
		{
			InvalidateFont(FontStyleBit);
			InvalidateTextTexture();
			InvalidateRender();
		}
	}

	void TextBlock::SetForeground(Color value)
	{
		if(SetProperty(ForegroundProperty, _foreground, value))
		{
			InvalidateTextTexture();
			InvalidateRender();
		}		
	}

	void TextBlock::SetTextWrapping(TextWrapping value)
	{
		if(SetProperty(TextWrappingProperty, _textWrapping, value))
		{
			InvalidateTextTexture();
			InvalidateMeasure();
		}		
	}

	void TextBlock::SetTextAlignment(TextAlignment value)
	{
		if(SetProperty(TextAlignmentProperty, _textAlignment, value))
		{
			InvalidateFont(TextAlignmentBit);
			InvalidateTextTexture();
			InvalidateMeasure();
		}		
	}

	void TextBlock::SetPadding(Thickness value)
	{
		if(SetProperty(PaddingProperty, _padding, value))
		{
			InvalidateMeasure();
		}
	}

	void TextBlock::Render(RenderContext context)
	{
		RenderBackground(context);

		if(!_isTextTextureValid)
		{
			CreateTextTexture();
		}

		if(_textTexture)
		{
			const auto& bounds = GetBounds();
			context.SetClip(Rect(bounds.x, bounds.y, bounds.w, bounds.h));
			context.DrawTexture(_textTexture, _textTextureBounds);
			context.ResetClip();
		}

		UIElement::Render(context);
	}

	void TextBlock::InvalidateTextTexture()
	{
		_isTextTextureValid = false;
	}

	FSize TextBlock::MeasureContent(FSize avaliableSize)
	{
		if(_text == "")
		{
			return FSize();
		}

		if(_fontValidationBits.any())
		{
			UpdateFont();
		}

		int width = 0;
		int height = 0;		

		switch(_textWrapping)
		{
			case Sgl::TextWrapping::NoWrap:
			{
				TTF_GetStringSize(_fontImpl, _text.data(), _text.length(), &width, &height);
				break;
			}

			case Sgl::TextWrapping::Wrap:
			{
				int wrapWidth = avaliableSize.Width;
				TTF_GetStringSizeWrapped(_fontImpl, _text.data(), _text.length(),
										 wrapWidth, &width, &height);
				break;
			}
		}

		_textTextureBounds = FRect(0, 0, width, height);

		return FSize
		{
			.Width = static_cast<float>(width) + _padding.Left + _padding.Right,
			.Height = static_cast<float>(height) + _padding.Top + _padding.Bottom
		};
	}

	void TextBlock::ArrangeContent(FRect rect)
	{
		_textTextureBounds.x = rect.x + _padding.Left;
		_textTextureBounds.y = rect.y + _padding.Top;
	}

	void TextBlock::InvalidateFont(size_t bit)
	{
		_fontValidationBits.set(bit);
	}

	void TextBlock::UpdateFont()
	{
		if(_fontValidationBits[FontFamilyBit])
		{
			_fontImpl = FontImpl(_fontFamily, _fontSize);
		}
		else if(_fontValidationBits[FontSizeBit])
		{
			_fontImpl.SetSize(_fontSize);
		}

		if(_fontValidationBits[FontStyleBit])
		{
			_fontImpl.SetStyle(_fontStyle);
		}

		if(_fontValidationBits[FontOutlineBit])
		{
			_fontImpl.SetOutline(_outline);
		}

		if(_fontValidationBits[FlowDirectionBit])
		{
			_fontImpl.SetFlowDirection(_flowDirection);
		}

		if(_fontValidationBits[TextAlignmentBit])
		{
			_fontImpl.SetTextAligment(_textAlignment);
		}

		_fontValidationBits = 0;
	}

	void TextBlock::CreateTextTexture()
	{
		if(_text != "")
		{
			auto renderer = GetVisualRoot()->GetRenderer();
			switch(_textWrapping)
			{
				case TextWrapping::NoWrap:
				{
					_textTexture = Texture(renderer, FontQuality::Blended, _fontImpl, _text, _foreground);
					break;
				}

				case TextWrapping::Wrap:
				{
					int wrapWidth = _textTextureBounds.w;
					_textTexture = Texture(renderer, FontQuality::Blended, _fontImpl, _text, wrapWidth, _foreground);
					break;
				}
			}
		}
		else
		{
			_textTexture = nullptr;
		}

		_isTextTextureValid = true;
	}
}