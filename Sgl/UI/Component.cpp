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
        if(hoverComponent && hoverComponent->IsPointIn(e.position))
        {
            hoverComponent->OnMouseMove(e);
            hoverComponent->children.OnMouseMove(e);
            return;
        }

        for(Component& component : *this)
        {
            if(component.IsPointIn(e.position))
            {
                if(hoverComponent)
                {
                    hoverComponent->OnMouseLeave(e);
                }

                hoverComponent = &component;
                component.OnMouseEnter(e);
                component.OnMouseMove(e);
                component.children.OnMouseMove(e);
                return;
            }
        }
    }

    void ComponentsCollection::OnMouseDown(const MouseButtonEventArgs& e)
    {
        if(hoverComponent)
        {
            hoverComponent->OnMouseDown(e);
            hoverComponent->children.OnMouseDown(e);
        }
    }

    void ComponentsCollection::OnMouseUp(const MouseButtonEventArgs& e)
    {
        if(hoverComponent)
        {
            hoverComponent->OnMouseUp(e);
            hoverComponent->children.OnMouseUp(e);
        }
    }

    void ComponentsCollection::OnRender(RenderContext renderContext) const
    {
        for(Component& component : *this)
        {
            component.OnRender(renderContext);
            component.children.OnRender(renderContext);
        }
    }

    Component::Component(UIElement& parent):
        parent(parent),
        scene(static_cast<Sgl::Scene&>(parent.GetRootElement()))
    {
        AddProperty<float>(widthProperty);
        AddProperty<float>(heightProperty);
        AddProperty<float>(minWidthProperty);
        AddProperty<float>(minHeightProperty);
        AddProperty<float>(maxWidthProperty);
        AddProperty<float>(minHeightProperty);
        AddProperty<size_t>(zIndexProperty, 1);
        AddProperty<Thikness>(marginProperty);
        AddProperty<const Component*>(toolTipProperty);
        AddProperty<HorizontalAlignment>(horizontalAlignmentProperty, HorizontalAlignment::Stretch);
        AddProperty<VerticalAligment>(verticalAligmentProperty, VerticalAligment::Stretch);
        AddProperty<Visibility>(visibilityProperty, Visibility::Visible);
        AddProperty<Color>(borderColorProperty, Colors::transparent);
        AddProperty<Thikness>(borderThiknessProperty);
        AddProperty<FontFamily>(fontFamilyProperty);
        AddProperty<FontWeight>(fontWeightProperty, FontWeight::Normal);
        AddProperty<Color>(fontColorProperty, Colors::black);
        AddProperty<uint16_t>(fontSizeProperty, defaultFontSize);
    }

    void Component::AddStyle(const Style& style)
    {
        UIElement::AddStyle(style);
    }

    void Component::SwitchCursorOn(const Cursor& cursor)
    {
        scene.SwitchCursorOn(cursor);
    }

    void Component::SwitchCursorOnDefault()
    {
        scene.SwitchCursorOnDefault();
    }

    UIElement& Component::GetRootElement()
    {
        return scene;
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
        SwitchCursorOn(GetCursor());
        MouseEnter.TryRaise(*this, e);
    }

    void Component::OnMouseLeave(const MouseButtonEventArgs& e)
    {
        _hover = false;
        MouseLeave.TryRaise(*this, e);
    }
}