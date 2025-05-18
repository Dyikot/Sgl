#include "UIElementsCollection.h"

namespace Sgl
{
    bool IsIntersects(FPoint point, const IUIElement& element)
    {
        auto position = element.GetPosition();

        return point.x >= position.x &&
               point.x <= position.x + element.GetWidth() &&
               point.y >= position.y &&
               point.y <= position.y + element.GetHeight();
    }

    UIElementsCollection::UIElementsCollection(IVisual& parent):
        Parent(parent)
    {}

    void UIElementsCollection::OnMouseMove(const MouseButtonEventArgs& e)
    {
        if(HoverElement && IsIntersects(e.Position, *HoverElement))
        {
            HoverElement->OnMouseMove(e);
            HoverElement->Children().OnMouseMove(e);
            return;
        }

        for(IUIElement& element : *this)
        {
            if(IsIntersects(e.Position, element))
            {
                if(HoverElement)
                {
                    HoverElement->OnMouseLeave(e);
                }

                HoverElement = &element;
                Cursor::Set(element.GetCursor());
                element.OnMouseEnter(e);
                element.OnMouseMove(e);
                element.Children().OnMouseMove(e);

                return;
            }
        }

        if(HoverElement)
        {
            HoverElement->OnMouseLeave(e);
        }

        HoverElement = nullptr;
        Cursor::Set(Parent.GetCursor());
    }

    void UIElementsCollection::OnMouseDown(const MouseButtonEventArgs& e)
    {
        if(HoverElement)
        {
            HoverElement->OnMouseDown(e);
            HoverElement->Children().OnMouseDown(e);
        }
    }

    void UIElementsCollection::OnMouseUp(const MouseButtonEventArgs& e)
    {
        if(HoverElement)
        {
            HoverElement->OnMouseUp(e);
            HoverElement->Children().OnMouseUp(e);
        }
    }

    void UIElementsCollection::OnMouseDoubleClick(const MouseButtonEventArgs& e)
    {}

    void UIElementsCollection::OnMouseWheel(const MouseWheelEventArgs& e)
    {}

    void UIElementsCollection::OnKeyDown(const KeyEventArgs& e)
    {
        for(IUIElement& element : *this)
        {
            element.OnKeyDown(e);
        }
    }

    void UIElementsCollection::OnKeyUp(const KeyEventArgs& e)
    {
        for(IUIElement& element : *this)
        {
            element.OnKeyUp(e);
        }
    }
}