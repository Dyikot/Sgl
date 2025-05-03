#include "ComponentsCollection.h"
#include "Component.h"
#include "../ECS/Systems.h"

namespace Sgl
{
    bool ZIndexComparer::operator()(const Component& left, const Component& right) const
    {
        return left.style->zIndex < right.style->zIndex;
    }

    constexpr bool IsIntersects(SDL_FPoint point, const Component& component)
    {
        return point.x >= component.position.x &&
            point.x <= component.position.x + component.style->width &&
            point.y >= component.position.y &&
            point.y <= component.position.y + component.style->height;
    }

    ComponentsCollection::ComponentsCollection(UIElement& parent):
        parent(parent)
    {}

    void ComponentsCollection::OnMouseMove(const MouseButtonEventArgs& e)
    {
        if(hoverComponent && IsIntersects(e.position, *hoverComponent))
        {
            hoverComponent->OnMouseMove(e);
            hoverComponent->children.OnMouseMove(e);
            return;
        }

        for(Component& component : *this)
        {
            if(IsIntersects(e.position, component))
            {
                if(hoverComponent)
                {
                    hoverComponent->OnMouseLeave(e);
                }

                hoverComponent = &component;
                SetCursor(component.style->cursor);
                component.OnMouseEnter(e);
                component.OnMouseMove(e);
                component.children.OnMouseMove(e);
                return;
            }
        }

        if(hoverComponent)
        {
            hoverComponent->OnMouseLeave(e);
        }

        hoverComponent = nullptr;
        SetCursor(parent.style->cursor);
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

    void ComponentsCollection::SetCursor(const Cursor& cursor)
    {
        if(SDL_GetCursor() != cursor)
        {
            SDL_SetCursor(cursor);
        }
    }
}