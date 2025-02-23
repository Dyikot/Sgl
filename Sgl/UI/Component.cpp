#include "Component.h"
#include "../Application.h"

namespace Sgl
{
    Component::Component(SDL_FPoint position):
        Position(position)
    {
        AddProperty<float>(WidthProperty);
        AddProperty<float>(HeightProperty);
        AddProperty<float>(MinWidthProperty);
        AddProperty<float>(MinHeightProperty);
        AddProperty<float>(MaxWidthProperty);
        AddProperty<float>(MinHeightProperty);
        AddProperty<size_t>(ZIndexProperty, 1);
        AddProperty<Thikness>(MarginProperty);
        AddProperty<const Sgl::Cursor*>(CursorProperty);
        AddProperty<IVisual*>(ToolTipProperty);
        AddProperty<HorizontalAlignment>(HorizontalAlignmentProperty, HorizontalAlignment::Stretch);
        AddProperty<VerticalAligment>(VerticalAligmentProperty, VerticalAligment::Stretch);
        AddProperty<Visibility>(VisibilityProperty, Visibility::Visible);
    }

    void Component::SetStyle(const Style& style)
    {
        UIElement::SetStyle(style);

        for(auto& [id, object] : style.Setters)
        {
            GetProperty(id).CopyValue(object);
        }
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

        if(auto cursor = GetCursor(); cursor)
        {
            Application::Current()->SetCursor(*cursor);
        }

        if(MouseEnter)
        {
            MouseEnter(this, e);
        }        
    }

    void Component::OnMouseLeave(const MouseButtonEventArgs& e)
    {
        _mouseOver = false;

        if(GetCursor())
        {
            Application::Current()->SetDefaultCursor();
        }

        if(MouseLeave)
        {
            MouseLeave(this, e);
        }
    }
}