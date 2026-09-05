#include "TextBlock.h"
#include "../Application.h"
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
		Name = "TextBlock";
	}

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
		_textBounds(other._textBounds),
		_fontImpl(std::move(other._fontImpl)),
		_textTexture(std::move(other._textTexture)),
		_fontFlags(other._fontFlags)
	{
	}

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

	void TextBlock::SetFontFamily(FontFamily value, ValueSource source)
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
		UIElement::Render(context);

		if(auto& textTexture = GetTextTexture(context.GetRenderer()))
		{
			auto [x, y, width, height] = GetBounds();
			context.SetClip(Rect(x, y, width, height));
			context.DrawTexture(textTexture, &_textBounds, nullptr);
			context.ResetClip();
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
		FSize size {};

		if(_text.empty())
		{
			return size;
		}

		if(_fontFlags > 0)
		{
			UpdateFont();
		}

		int width = 0;
		int height = 0;		

		auto [left, top, right, bottom] = GetPadding();

		if(_textWrapping == TextWrapping::NoWrap)
		{
			TTF_GetStringSize(_fontImpl, _text.data(), _text.length(), &width, &height);
		}
		else
		{
			int wrapWidth = availableSize.Width;
			TTF_GetStringSizeWrapped(_fontImpl, _text.data(), _text.length(), wrapWidth, &width, &height);
		}

		_textBounds = FRect(0, 0, width, height);
		size.Width = width + left + right;
		size.Height = height + top + bottom;

		return size;
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
			_fontImpl = Font(_fontFamily, _fontSize);
		}
		else if(_fontFlags & FontSizeFlag)
		{
			_fontImpl.SetSize(_fontSize);
		}

		if(_fontFlags & FontStyleFlag)
		{
			_fontImpl.SetStyle(_fontStyle);
		}

		if(_fontFlags & FontOutlineFlag)
		{
			_fontImpl.SetOutline(_outline);
		}

		if(_fontFlags & FlowDirectionFlag)
		{
			_fontImpl.SetFlowDirection(_flowDirection);
		}

		if(_fontFlags & TextAlignmentFlag)
		{
			_fontImpl.SetTextAligment(_textAlignment);
		}

		_fontFlags = 0;
	}

	Texture& TextBlock::GetTextTexture(SDL_Renderer* renderer)
	{
		if(!_textTexture && !_text.empty())
		{
			_textTexture = _textWrapping == TextWrapping::NoWrap
				? Texture(renderer, FontQuality::Blended, _fontImpl, _text, _foreground)
				: Texture(renderer, FontQuality::Blended, _fontImpl, _text, _textBounds.w, _foreground);
		}

		return _textTexture;
	}
}

namespace Sgl
{
	ResourceSetter<UIElements::TextBlock, Color>::ResourceSetter(
		ForegroundProperty& property,
		ResourceKey key):
		Setter(property),
		_key(std::move(key))
	{}

	void ResourceSetter<UIElements::TextBlock, Color>::Apply(
		Styleable& target,
		ValueSource valueSource) const
	{
		auto& property = static_cast<ForegroundProperty&>(GetProperty());
		property.InvokeSetter(
			static_cast<UIElements::TextBlock&>(target),
			App->Resources.GetColor(_key.Value),
			valueSource
		);
	}
}