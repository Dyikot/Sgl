#include "Component.h"

namespace Sgl
{
    Component::Component()
    {
        AddProperty<float>(WidthProperty);
        AddProperty<float>(HeightProperty);
        AddProperty<float>(MinWidthProperty);
        AddProperty<float>(MinHeightProperty);
        AddProperty<float>(MaxWidthProperty);
        AddProperty<float>(MinHeightProperty);
        AddProperty<size_t>(ZIndexProperty, 1);
        AddProperty<Thikness>(MarginProperty);
        AddProperty<const Component*>(ToolTipProperty);
        AddProperty<HorizontalAlignment>(HorizontalAlignmentProperty, HorizontalAlignment::Stretch);
        AddProperty<VerticalAligment>(VerticalAligmentProperty, VerticalAligment::Stretch);
        AddProperty<Visibility>(VisibilityProperty, Visibility::Visible);
        AddProperty<Color>(BorderColorProperty, Colors::Transparent);
        AddProperty<Thikness>(BorderThiknessProperty);
        AddProperty<FontFamily>(FontFamilyProperty);
        AddProperty<FontWeight>(FontWeightProperty, FontWeight::Normal);
        AddProperty<Color>(FontColorProperty, Colors::Black);
        AddProperty<uint16_t>(FontSizeProperty, DefaultFontSize);
    }

    void Component::AddStyle(const Style& style)
    {
        UIElement::AddStyle(style);
    }

    void Component::OnRender(RenderContext renderContext) const
    {
        if(auto tooltip = GetToolTip(); _hover && tooltip)
        {
            tooltip->OnRender(renderContext);
        }
    }

    void Component::OnMouseEnter(const MouseButtonEventArgs& e)
    {
        _hover = true;
        mouseEnter.TryRaise(*this, e);
    }

    void Component::OnMouseLeave(const MouseButtonEventArgs& e)
    {
        _hover = false;
        mouseLeave.TryRaise(*this, e);
    }
}