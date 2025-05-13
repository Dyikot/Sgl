#include "UIElement.h"

namespace Sgl
{
    bool ZIndexComparer::operator()(const UIElement& left, const UIElement& right) const
    {
        return left.Properties.ZIndex < right.Properties.ZIndex;
    }

    bool IsIntersects(SDL_FPoint point, const UIElement& component)
    {
        return point.x >= component.Position.x &&
               point.x <= component.Position.x + component.Properties.Width &&
               point.y >= component.Position.y &&
               point.y <= component.Position.y + component.Properties.Height;
    }

    void UIElementsCollection::OnMouseMove(const MouseButtonEventArgs& e)
    {
        if(HoverElement && IsIntersects(e.Position, *HoverElement))
        {
            HoverElement->OnMouseMove(e);
            HoverElement->Children.OnMouseMove(e);
            return;
        }

        for(UIElement& component : *this)
        {
            if(IsIntersects(e.Position, component))
            {
                if(HoverElement)
                {
                    HoverElement->OnMouseLeave(e);
                }

                HoverElement = &component;
                Cursor::Set(component.Properties.Cursor);
                component.OnMouseEnter(e);
                component.OnMouseMove(e);
                component.Children.OnMouseMove(e);
                return;
            }
        }

        if(HoverElement)
        {
            HoverElement->OnMouseLeave(e);
        }

        HoverElement = nullptr;
       
    }

    void UIElementsCollection::OnMouseDown(const MouseButtonEventArgs& e)
    {
        if(HoverElement)
        {
            HoverElement->OnMouseDown(e);
            HoverElement->Children.OnMouseDown(e);
        }
    }

    void UIElementsCollection::OnMouseUp(const MouseButtonEventArgs& e)
    {
        if(HoverElement)
        {
            HoverElement->OnMouseUp(e);
            HoverElement->Children.OnMouseUp(e);
        }
    }

    void UIElementsCollection::OnRender(RenderContext renderContext) const
    {
        for(UIElement& component : *this)
        {
            component.OnRender(renderContext);
            component.Children.OnRender(renderContext);
        }
    }

    void UIElementsCollection::OnMouseDoubleClick(const MouseButtonEventArgs& e)
    {}

    void UIElementsCollection::OnMouseWheel(const MouseWheelEventArgs& e)
    {}

    void UIElementsCollection::OnKeyDown(const KeyEventArgs& e)
    {
        for(UIElement& component : *this)
        {
            component.OnKeyDown(e);
        }
    }

    void UIElementsCollection::OnKeyUp(const KeyEventArgs& e)
    {
        for(UIElement& component : *this)
        {
            component.OnKeyUp(e);
        }
    }

    UIElement::UIElement():
        Position(), 
        Style(Properties)
    {
        Style.AddState(StyleState::Hover);
    }

    void UIElement::OnRender(RenderContext renderContext) const
    {
        if(_isHover && Properties.Tooltip)
        {
            Properties.Tooltip->OnRender(renderContext);
        }
    }
}