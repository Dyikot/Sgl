#include "Component.h"
#include "../Application.h"

namespace Sgl
{
    Component::Component():
        Component(SDL_FPoint())
    {}

    Component::Component(SDL_FPoint position) noexcept:
        Position(position),
        _isEventsInitialized(InitializeEvents()),
        MouseEnter(GetEventValue<MouseEventHandler>(MouseEnterEvent)),
        MouseLeave(GetEventValue<MouseEventHandler>(MouseLeaveEvent))
    {
        AddProperty<float>(WidthProperty);
        AddProperty<float>(HeightProperty);
        AddProperty<float>(MinWidthProperty);
        AddProperty<float>(MinHeightProperty);
        AddProperty<float>(MaxWidthProperty);
        AddProperty<float>(MinHeightProperty);
        AddProperty<size_t>(ZIndexProperty, 1);
        AddProperty<Thikness>(MarginProperty);
        AddProperty<const Sgl::Cursor*>(CursorProperty, nullptr);
        AddProperty<IVisual*>(ToolTipProperty, nullptr);
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
        if(_isMouseOver && ToolTip)
        {
            ToolTip->OnRender(renderContext);
        }
    }

    void Component::OnPropertyChanged(PropertyId id)
    {
        _bindings.UpdateSource(id);
    }

    void Component::ClearBinding(PropertyId id)
    {
        _bindings.erase(id);
    }

    void Component::ClearBindings()
    {
        _bindings.clear();
    }

    void Component::OnMouseEnter(const MouseButtonEventArgs& e)
    {
        _isMouseOver = true;

        if(Cursor)
        {
            Application::Current()->SetCursor(*Cursor);
        }

        if(MouseEnter)
        {
            MouseEnter(this, e);
        }        
    }

    void Component::OnMouseLeave(const MouseButtonEventArgs& e)
    {
        _isMouseOver = false;

        if(Cursor)
        {
            Application::Current()->SetDefaultCursor();
        }

        if(MouseLeave)
        {
            MouseLeave(this, e);
        }
    }

    bool Component::InitializeEvents()
    {
        AddEvent<MouseEventHandler>(MouseEnterEvent);
        AddEvent<MouseEventHandler>(MouseLeaveEvent);

        return true;
    }
}