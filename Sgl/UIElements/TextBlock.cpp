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
		_fontFlags(FontFamilyFlag)
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
		_fontFlags(other._fontFlags)
	{}

	void TextBlock::SetText(const std::string& value, ValueSource source)
	{
		if(SetProperty(TextProperty, _text, value, _textSource, source))
		{
			InvalidateTextTexture();
			InvalidateMeasure();
		}
	}

	void TextBlock::SetFontSize(float value, ValueSource source)
	{
		if(SetProperty(FontSizeProperty, _fontSize, value, _fontSizeSource, source))
		{
			InvalidateFont(FontSizeFlag);
			InvalidateTextTexture();
			InvalidateMeasure();
		}
	}

	void TextBlock::SetFontOutline(int value, ValueSource source)
	{
		if(SetProperty(FontOutlineProperty, _outline, value, _outlineSource, source))
		{
			InvalidateFont(FontOutlineFlag);
			InvalidateTextTexture();
			InvalidateMeasure();
		}
	}

	void TextBlock::SetFontFamily(const FontFamily& value, ValueSource source)
	{
		if(SetProperty(FontFamilyProperty, _fontFamily, value, _fontFamilySource, source))
		{
			InvalidateFont(FontFamilyFlag);
			InvalidateTextTexture();
			InvalidateMeasure();
		}
	}

	void TextBlock::SetFlowDirection(FlowDirection value, ValueSource source)
	{
		if(SetProperty(FlowDirectionProperty, _flowDirection, value, _flowDirectionSource, source))
		{
			InvalidateFont(FlowDirectionFlag);
			InvalidateTextTexture();
			InvalidateMeasure();
		}
	}

	void TextBlock::SetFontStyle(FontStyle value, ValueSource source)
	{
		if(SetProperty(FontStyleProperty, _fontStyle, value, _fontStyleSource, source))
		{
			InvalidateFont(FontStyleFlag);
			InvalidateTextTexture();
			InvalidateRender();
		}
	}

	void TextBlock::SetForeground(Color value, ValueSource source)
	{
		if(SetProperty(ForegroundProperty, _foreground, value, _foregroundSource, source))
		{
			InvalidateTextTexture();
			InvalidateRender();
		}
	}

	void TextBlock::SetTextWrapping(TextWrapping value, ValueSource source)
	{
		if(SetProperty(TextWrappingProperty, _textWrapping, value, _textWrappingSource, source))
		{
			InvalidateTextTexture();
			InvalidateMeasure();
		}
	}

	void TextBlock::SetTextAlignment(TextAlignment value, ValueSource source)
	{
		if(SetProperty(TextAlignmentProperty, _textAlignment, value, _textAlignmentSource, source))
		{
			InvalidateFont(TextAlignmentFlag);
			InvalidateTextTexture();
			InvalidateMeasure();
		}
	}

	void TextBlock::SetPadding(Thickness value, ValueSource source)
	{
		if(SetProperty(PaddingProperty, _padding, value, _paddingSource, source))
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

		if(_fontFlags.any())
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

	void TextBlock::InvalidateFont(size_t flag)
	{
		_fontFlags.set(flag);
	}

	void TextBlock::UpdateFont()
	{
		if(_fontFlags[FontFamilyFlag])
		{
			_fontImpl = FontImpl(_fontFamily, _fontSize);
		}
		else if(_fontFlags[FontSizeFlag])
		{
			_fontImpl.SetSize(_fontSize);
		}

		if(_fontFlags[FontStyleFlag])
		{
			_fontImpl.SetStyle(_fontStyle);
		}

		if(_fontFlags[FontOutlineFlag])
		{
			_fontImpl.SetOutline(_outline);
		}

		if(_fontFlags[FlowDirectionFlag])
		{
			_fontImpl.SetFlowDirection(_flowDirection);
		}

		if(_fontFlags[TextAlignmentFlag])
		{
			_fontImpl.SetTextAligment(_textAlignment);
		}

		_fontFlags = 0;
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