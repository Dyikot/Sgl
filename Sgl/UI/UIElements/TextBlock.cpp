#include "TextBlock.h"
#include <algorithm>

namespace Sgl::UIElements
{
	TextBlock::TextBlock(const TextBlock& other):
		UIElement(other),
		_text(other._text),
		_fontSize(other._fontSize),
		_fontFamily(other._fontFamily),
		_fontStyle(other._fontStyle),
		_foreground(other._foreground),
		_textWrapping(other._textWrapping),
		_textAlignment(other._textAlignment),
		_padding(other._padding),
		_textTextureBounds(other._textTextureBounds),
		_fontImpl(),
		_textTexture(),
		_isTextTextureValid(),
		_fontValidationFlags(FontFamilyFlag)
	{}

	TextBlock::TextBlock(TextBlock&& other) noexcept:
		UIElement(std::move(other)),
		_text(std::move(other._text)),
		_fontSize(other._fontSize),
		_fontFamily(std::move(other._fontFamily)),
		_fontStyle(other._fontStyle),
		_foreground(other._foreground),
		_textWrapping(other._textWrapping),
		_textAlignment(other._textAlignment),
		_padding(other._padding),
		_textTextureBounds(other._textTextureBounds),
		_fontImpl(std::move(other._fontImpl)),
		_textTexture(std::move(other._textTexture)),
		_isTextTextureValid(other._isTextTextureValid),
		_fontValidationFlags(other._fontValidationFlags)
	{}

	void TextBlock::Render(RenderContext context)
	{
		RenderBackground(context);
		CreateTextTextureIfInvalid(_textTextureBounds.w);

		if(_textTexture)
		{
			context.DrawTexture(*_textTexture, _textTextureBounds);
		}

		UIElement::Render(context);
	}

	FSize TextBlock::MeasureContent(FSize avaliableSize)
	{
		CreateTextTextureIfInvalid(GetWidth());

		if(_textTexture)
		{
			auto [width, height] = _textTexture->GetSize();

			return FSize
			{
				.Width = static_cast<float>(width) + _padding.Left + _padding.Right,
				.Height = static_cast<float>(height) + _padding.Top + _padding.Bottom
			};
		}

		return FSize();
	}

	void TextBlock::ArrangeContent(FRect rect)
	{
		if(_textTexture)
		{
			Size textureSize = _textTexture->GetSize();
			float width = std::fmin(rect.w - _padding.Left - _padding.Right, textureSize.Width);
			float height = std::fmin(rect.h - _padding.Top - _padding.Bottom, textureSize.Height);

			if(width < 0)
			{
				width = 0;
			}

			if(height < 0)
			{
				height = 0;
			}

			_textTextureBounds =
			{
				.x = rect.x + _padding.Left,
				.y = rect.y + _padding.Top,
				.w = width,
				.h = height
			};
		}
	}

	void TextBlock::CreateOrUpdateFont()
	{
		const auto flags = _fontValidationFlags.to_ulong();

		if(flags & FontFamilyFlag)
		{
			_fontImpl = FontImpl(_fontFamily, _fontSize);
			_fontImpl.SetStyle(_fontStyle);
			return;
		}
		
		if(flags & FontSizeFlag)
		{
			_fontImpl.SetSize(_fontSize);
		}

		if(flags & FontStyleFlag)
		{
			_fontImpl.SetStyle(_fontStyle);
		}
	}

	void TextBlock::CreateTextTextureIfInvalid(float maxLineWidth)
	{
		if(_fontValidationFlags.any())
		{
			CreateOrUpdateFont();
			_fontValidationFlags = 0;
		}

		if(!_isTextTextureValid)
		{
			TTF_SetFontWrappedAlign(_fontImpl, static_cast<int>(_textAlignment));

			if(_text != "")
			{
				if(_textWrapping == TextWrapping::NoWrap)
				{
					_textTexture = NewUnique<Texture>(
						Surface(FontRenderType::Blended, _fontImpl, _text, _foreground));
				}
				else
				{
					_textTexture = NewUnique<Texture>(
						Surface(FontRenderType::Blended, _fontImpl, _text, maxLineWidth, _foreground));
				}
			}

			_isTextTextureValid = true;
		}
	}
}