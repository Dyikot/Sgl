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
		_property(property),
		_key(std::move(key))
	{}

	void ResourceSetter<UIElements::TextBlock, Color>::Apply(
		Styleable& target,
		ValueSource valueSource) const
	{
		auto& owner = static_cast<UIElements::TextBlock&>(target);
		_property.InvokeSetter(owner, App->Resources.GetColor(_key.Value), valueSource);
	}

	ISavedValue* ResourceSetter<UIElements::TextBlock, Color>::Save(Styleable& target) const
	{
		auto& owner = static_cast<UIElements::TextBlock&>(target);
		return new SavedPropertyValue<UIElements::TextBlock, Color>(_property, owner);
	}
}