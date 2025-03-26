#include "Component.h"
#include "Scene.h"

namespace Sgl
{
    bool ZIndexComparer::operator()(const Component& left, const Component& right) const
    {
        return left.GetZIndex() < right.GetZIndex();
    }

    void ComponentsCollection::OnMouseMove(const MouseButtonEventArgs& e)
    {
        if(HoverComponent && HoverComponent->IsPointIn(e.Position))
        {
            HoverComponent->OnMouseMove(e);
            HoverComponent->Children.OnMouseMove(e);
            return;
        }

        for(Component& component : *this)
        {
            if(component.IsPointIn(e.Position))
            {
                if(HoverComponent)
                {
                    HoverComponent->OnMouseLeave(e);
                }

                HoverComponent = &component;
                component.OnMouseEnter(e);
                component.OnMouseMove(e);
                component.Children.OnMouseMove(e);
                return;
            }
        }
    }

    void ComponentsCollection::OnMouseDown(const MouseButtonEventArgs& e)
    {
        if(HoverComponent)
        {
            HoverComponent->OnMouseDown(e);
            HoverComponent->Children.OnMouseDown(e);
        }
    }

    void ComponentsCollection::OnMouseUp(const MouseButtonEventArgs& e)
    {
        if(HoverComponent)
        {
            HoverComponent->OnMouseUp(e);
            HoverComponent->Children.OnMouseDown(e);
        }
    }

    void ComponentsCollection::OnRender(RenderContext& renderContext) const
    {
        for(Component& component : *this)
        {
            component.OnRender(renderContext);
            component.Children.OnRender(renderContext);
        }
    }

    Component::Component(UIElement& parent)
        : Parent(parent), Scene(static_cast<Sgl::Scene&>(parent.GetRootElement()))
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

    void Component::SwitchCursorOn(const Cursor& cursor)
    {
        Scene.SwitchCursorOn(cursor);
    }

    void Component::SwitchCursorOnDefault()
    {
        Scene.SwitchCursorOnDefault();
    }

    UIElement& Component::GetRootElement()
    {
        return Scene;
    }

    bool Component::IsPointIn(SDL_FPoint point) const
    {
        return point.x >= Position.x &&
               point.x <= Position.x + GetWidth() &&
               point.y >= Position.y &&
               point.y <= Position.y + GetHeight();
    }

    void Component::OnRender(RenderContext& renderContext) const
    {
        if(auto tooltip = GetToolTip(); _hover && tooltip)
        {
            tooltip->OnRender(renderContext);
        }
    }

    void Component::OnMouseEnter(const MouseButtonEventArgs& e)
    {
        _hover = true;
        SwitchCursorOn(GetCursor());
        MouseEnter.TryRaise(*this, e);
    }

    void Component::OnMouseLeave(const MouseButtonEventArgs& e)
    {
        _hover = false;
        MouseLeave.TryRaise(*this, e);
    }
}