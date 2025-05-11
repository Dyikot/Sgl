#include "ComponentsCollection.h"
#include "Component.h"
#include "../ECS/Systems.h"

namespace Sgl
{
    bool ZIndexComparer::operator()(const Component& left, const Component& right) const
    {
        return left.Style.ZIndex < right.Style.ZIndex;
    }

    bool IsIntersects(SDL_FPoint point, const Component& component)
    {
        return point.x >= component.Position.x &&
               point.x <= component.Position.x + component.Style.Width &&
               point.y >= component.Position.y &&
               point.y <= component.Position.y + component.Style.Height;
    }

    ComponentsCollection::ComponentsCollection(UIElement& parent):
        Parent(parent)
    {}

    void ComponentsCollection::OnMouseMove(const MouseButtonEventArgs& e)
    {
        if(HoverComponent && IsIntersects(e.Position, *HoverComponent))
        {
            HoverComponent->OnMouseMove(e);
            HoverComponent->Children.OnMouseMove(e);
            return;
        }

        for(Component& component : *this)
        {
            if(IsIntersects(e.Position, component))
            {
                if(HoverComponent)
                {
                    HoverComponent->OnMouseLeave(e);
                }

                HoverComponent = &component;
                SetCursor(component.Style.Cursor);
                component.OnMouseEnter(e);
                component.OnMouseMove(e);
                component.Children.OnMouseMove(e);
                return;
            }
        }

        if(HoverComponent)
        {
            HoverComponent->OnMouseLeave(e);
        }

        HoverComponent = nullptr;
        SetCursor(Parent.Style.Cursor);
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
            HoverComponent->Children.OnMouseUp(e);
        }
    }

    void ComponentsCollection::OnRender(RenderContext renderContext) const
    {
        for(Component& component : *this)
        {
            component.OnRender(renderContext);
            component.Children.OnRender(renderContext);
        }
    }

    void ComponentsCollection::SetCursor(CursorGetter cursorGetter)
    {
        const auto& cursor = cursorGetter();
        if(SDL_GetCursor() != cursor)
        {
            SDL_SetCursor(cursor);
        }
    }
}