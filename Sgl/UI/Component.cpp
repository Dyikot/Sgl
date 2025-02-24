#include "Component.h"
#include "../Application.h"

namespace Sgl
{
    Component::Component(UIElement& parent):
        Parent(parent)
    {
        AddProperty<float>(WidthProperty);
        AddProperty<float>(HeightProperty);
        AddProperty<float>(MinWidthProperty);
        AddProperty<float>(MinHeightProperty);
        AddProperty<float>(MaxWidthProperty);
        AddProperty<float>(MinHeightProperty);
        AddProperty<size_t>(ZIndexProperty, 1);
        AddProperty<Thikness>(MarginProperty);
        AddProperty<IVisual*>(ToolTipProperty);
        AddProperty<HorizontalAlignment>(HorizontalAlignmentProperty, HorizontalAlignment::Stretch);
        AddProperty<VerticalAligment>(VerticalAligmentProperty, VerticalAligment::Stretch);
        AddProperty<Visibility>(VisibilityProperty, Visibility::Visible);
    }

    void Component::AddStyle(const Style& style)
    {
        UIElement::AddStyle(style);
    }

    void Component::SwitchCursorOn(const Cursor& cursor)
    {
        Parent.SwitchCursorOn(cursor);
    }

    void Component::SwitchCursorOnDefault()
    {
        Parent.SwitchCursorOnDefault();
    }

    void Component::OnRender(RenderContext& renderContext)
    {
        auto tooltip = GetPropertyValue<IVisual*>(ToolTipProperty);

        if(_mouseOver && tooltip)
        {
            tooltip->OnRender(renderContext);
        }
    }

    void Component::OnPropertyChanged(PropertyId id)
    {
        if(auto found = _bindings.find(id); found != _bindings.end())
        {
            found->second.NotifyTarget(GetProperty(id));
        }
    }

    void Component::OnMouseEnter(const MouseButtonEventArgs& e)
    {
        _mouseOver = true;
        SwitchCursorOn(GetCursor());

        if(MouseEnter)
        {
            MouseEnter(this, e);
        }        
    }

    void Component::OnMouseLeave(const MouseButtonEventArgs& e)
    {
        _mouseOver = false;


        if(MouseLeave)
        {
            MouseLeave(this, e);
        }
    }
}