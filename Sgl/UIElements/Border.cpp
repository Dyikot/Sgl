#include "Border.h"
#include "../Application.h"

namespace Sgl::UIElements
{
	Border::Border(const Border& other):
		ContentUIElement(other),
		_borderWidth(other._borderWidth),
		_borderColor(other._borderColor)
	{}

	Border::Border(Border&& other) noexcept:
		ContentUIElement(std::move(other)),
		_borderWidth(other._borderWidth),
		_borderColor(other._borderColor)
	{}

	void Border::SetBorderWidth(uint32_t value, ValueSource source)
	{
		if(SetProperty(BorderWidthProperty, _borderWidth, value, _borderWidthSource, source))
		{
			InvalidateMeasure();
		}
	}

	void Border::SetBorderColor(Color value, ValueSource source)
	{
		if(SetProperty(BorderColorProperty, _borderColor, value, _borderColorSource, source))
		{
			InvalidateRender();
		}
	}

	void Border::Render(RenderContext context)
	{
		RenderBackground(context);

		if(!_borderColor.IsTransparent())
		{			
			if(_borderWidth == 1u)
			{
				context.DrawRectange(_bounds, _borderColor);
			}
			else if(_borderWidth > 1u)
			{
				float width = _borderWidth;
				auto [x, y, w, h] = _bounds;
				FRect sides[] =
				{
					{ x, y, w, width },										// top
					{ x, y + h - width, w, width },							// bottom
					{ x, y + width, width, h - 2 * width },					// left
					{ x + w - width, y + width, width, h - 2 * width },		// right
				};

				context.DrawRectanglesFill(sides, _borderColor);
			}
		}

		ContentUIElement::Render(context);
	}

	Thickness Border::GetLayoutPadding() const
	{
		return ContentUIElement::GetLayoutPadding() + Thickness(_borderWidth);
	}
}

namespace Sgl
{
	ResourceSetter<UIElements::Border, Color>::ResourceSetter(
		StyleableProperty<UIElements::Border, Color>& property,
		ResourceKey key):
		SetterBase(property),
		_key(std::move(key))
	{}

	void ResourceSetter<UIElements::Border, Color>::Apply(
		StyleableElement& target,
		ValueSource valueSource) const
	{
		auto& property = static_cast<StyleableProperty<UIElements::Border, Color>&>(GetProperty());
		property.InvokeSetter(
			static_cast<UIElements::Border&>(target),
			App->Resources.GetColor(_key.GetName()),
			valueSource
		);
	}
}