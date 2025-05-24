#include "UIElement.h"

namespace Sgl
{
    bool IsIntersects(FPoint point, const UIElementsCollection::UIElementContext& context)
    {
        return point.x >= context.Position.x &&
               point.x <= context.Position.x + context.Element.Width &&
               point.y >= context.Position.y &&
               point.y <= context.Position.y + context.Element.Height;
    }

    UIElementsCollection::UIElementsCollection(VisualElement& parent):
        Parent(parent)
    {}

    void UIElementsCollection::OnMouseMove(const MouseButtonEventArgs& e)
    {
        if(HoverElementContext && IsIntersects(e.Position, *HoverElementContext))
        {
            HoverElementContext->Element.OnMouseMove(e);
            HoverElementContext->Element.Children.OnMouseMove(e);
            return;
        }

        for(auto& context : _items)
        {
            if(IsIntersects(e.Position, context))
            {
                if(HoverElementContext)
                {
                    HoverElementContext->Element.OnMouseLeave(e);
                }

                HoverElementContext = &context;
                Cursor::Set(context.Element.Cursor);
                context.Element.OnMouseEnter(e);
                context.Element.OnMouseMove(e);
                context.Element.Children.OnMouseMove(e);

                return;
            }
        }

        if(HoverElementContext)
        {
            HoverElementContext->Element.OnMouseLeave(e);
        }

        HoverElementContext = nullptr;
        Cursor::Set(Parent.Cursor);
    }

    void UIElementsCollection::OnMouseDown(const MouseButtonEventArgs& e)
    {
        if(HoverElementContext)
        {
            HoverElementContext->Element.OnMouseDown(e);
            HoverElementContext->Element.Children.OnMouseDown(e);
        }
    }

    void UIElementsCollection::OnMouseUp(const MouseButtonEventArgs& e)
    {
        if(HoverElementContext)
        {
            HoverElementContext->Element.OnMouseUp(e);
            HoverElementContext->Element.Children.OnMouseUp(e);
        }
    }

    void UIElementsCollection::OnMouseDoubleClick(const MouseButtonEventArgs& e)
    {}

    void UIElementsCollection::OnMouseWheel(const MouseWheelEventArgs& e)
    {}

    void UIElementsCollection::OnKeyDown(const KeyEventArgs& e)
    {
        for(auto& context : _items)
        {
            context.Element.OnKeyDown(e);
        }
    }

    void UIElementsCollection::OnKeyUp(const KeyEventArgs& e)
    {
        for(auto& context : _items)
        {
            context.Element.OnKeyUp(e);
        }
    }

    bool UIElementsCollection::ZIndexComparer::operator()(const UIElementContext& left, const UIElementContext& right) const
    {
        return left.Element.ZIndex < right.Element.ZIndex;
    }
}