#include "Layout.h"

namespace Sgl::UI
{
    bool IsIntersects(FPoint point, const Layout::UIElementContext& context)
    {
        return point.x >= context.Position.x &&
               point.x <= context.Position.x + context.Element.Width &&
               point.y >= context.Position.y &&
               point.y <= context.Position.y + context.Element.Height;
    }

    Layout::Layout(VisualElement& parent):
        Parent(parent)
    {}

    void Layout::OnMouseMove(const MouseButtonEventArgs& e)
    {
        MouseMove.TryRaise(*this, e);

        if(_hoverElementContext && IsIntersects(e.Position, *_hoverElementContext))
        {
            _hoverElementContext->Element.OnMouseMove(e);
            //_hoverElement->Children.OnMouseMove(e);
            return;
        }

        for(auto& context : _children)
        {
            if(IsIntersects(e.Position, context))
            {
                if(_hoverElementContext)
                {
                    _hoverElementContext->Element.OnMouseLeave(e);
                }

                _hoverElementContext = &context;
                Cursor::Set(context.Element.Cursor);
                context.Element.OnMouseEnter(e);
                context.Element.OnMouseMove(e);
                //context.Children.OnMouseMove(e);

                return;
            }
        }

        if(_hoverElementContext)
        {
            _hoverElementContext->Element.OnMouseLeave(e);
        }

        _hoverElementContext = nullptr;
        Cursor::Set(Parent.Cursor);
    }
}