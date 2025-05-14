#include "UIElement.h"

namespace Sgl
{
    bool ZIndexComparer::operator()(const UIElement& left, const UIElement& right) const
    {
        return left.Properties.ZIndex < right.Properties.ZIndex;
    }

    bool IsIntersects(FPoint point, const UIElement& element)
    {
        return point.x >= element.Position.x &&
               point.x <= element.Position.x + element.Properties.Width &&
               point.y >= element.Position.y &&
               point.y <= element.Position.y + element.Properties.Height;
    }

    void UIElementsCollection::OnMouseMove(const MouseButtonEventArgs& e)
    {
        if(HoverElement && IsIntersects(e.Position, *HoverElement))
        {
            HoverElement->OnMouseMove(e);
            HoverElement->Children.OnMouseMove(e);
            return;
        }

        for(UIElement& element : *this)
        {
            if(IsIntersects(e.Position, element))
            {
                if(HoverElement)
                {
                    HoverElement->OnMouseLeave(e);
                }

                HoverElement = &element;
                Cursor::Set(element.Properties.Cursor);
                element.OnMouseEnter(e);
                element.OnMouseMove(e);
                element.Children.OnMouseMove(e);
                return;
            }
        }

        if(HoverElement)
        {
            HoverElement->OnMouseLeave(e);
        }

        HoverElement = nullptr;
        Cursor::Set(Parent ? Parent->Properties.Cursor : DefaultCursorGetter());
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

    UIElementsCollection::UIElementsCollection(UIElement* parent):
        Parent(parent)
    {}

    UIElementsCollection::UIElementsCollection(Func<Cursor::Getter> defaultCursorGetter):
        DefaultCursorGetter(std::move(defaultCursorGetter))
    {}

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
        BaseStyle(Properties),
        HoverStyle(Properties, BaseStyle),
        Children(this)
    {}

    void UIElement::OnRender(RenderContext renderContext) const
    {
        if(_isHover && Properties.Tooltip)
        {
            Properties.Tooltip->OnRender(renderContext);
        }
    }
}