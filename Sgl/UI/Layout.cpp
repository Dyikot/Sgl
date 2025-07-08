#include "Layout.h"

namespace Sgl
{  
    Layout::Layout():
        Children(*this),
        _isHover(false),
        _shouldArrange(false)
    {}

    static bool IsHover(FPoint point, FPoint position, float width, float height)
    {
        return point.x >= position.x &&
               point.x <= position.x + width &&
               point.y >= position.y &&
               point.y <= position.y + height;
    }

    void Layout::QueryArrange()
    {
        _shouldArrange = true;
    }

    void Sgl::Layout::OnRender(RenderContext context) const
    {
        UIElement::OnRender(context);
        
        for(auto& child : Children)
        {
            child->OnRender(context);
        }
    }

    void Layout::ApplyStyle()
    {
        StyleableElement::ApplyStyle();

        for(auto& child : Children)
        {
            child->ApplyStyle();
        }
    }

    void Layout::OnMouseMove(const MouseEventArgs& e)
    {
        bool wasHover = _isHover;
        _isHover = IsHover(e.Position, GetActualPosition(), GetActualWidth(), GetActualHeight());

        if(_isHover)
        {
            if(!wasHover)
            {
                OnMouseEnter(e);
            }

            OnChildrenMouseMove(e);
        }
        else if(wasHover)
        {
            OnMouseLeave(e);
        }
    }

    void Layout::OnMouseDown(const MouseButtonEventArgs& e)
    {
        UIElement::OnMouseDown(e);

        if(_hoverChild)
        {
            _hoverChild->OnMouseDown(e);
        }
    }

    void Layout::OnMouseUp(const MouseButtonEventArgs& e)
    {
        UIElement::OnMouseUp(e);

        if(_hoverChild)
        {
            _hoverChild->OnMouseUp(e);
        }
    }

    void Layout::OnMouseWheelChanged(const MouseWheelEventArgs& e)
    {
        for(auto& child : Children)
        {
            child->OnMouseWheelChanged(e);
        }
    }

    void Layout::OnKeyDown(const KeyEventArgs& e)
    {
        for(auto& child : Children)
        {
            child->OnKeyDown(e);
        }
    }

    void Layout::OnKeyUp(const KeyEventArgs& e)
    {
        for(auto& child : Children)
        {
            child->OnKeyUp(e);
        }
    }

    void Layout::OnSizeChanged()
    {
        UIElement::OnSizeChanged();

        if(Children.Count() > 0)
        {
            QueryArrange();
        }
    }

    void Layout::OnChildrenMouseMove(const MouseEventArgs& e)
    {
        if(_hoverChild && IsHover(e.Position, _hoverChild->GetActualPosition(),
                                  _hoverChild->GetActualWidth(),
                                  _hoverChild->GetActualHeight()))
        {
            _hoverChild->OnMouseMove(e);
            return;
        }

        for(auto& child : Children)
        {
            if(IsHover(e.Position, child->GetActualPosition(),
                       child->GetActualWidth(),
                       child->GetActualHeight()))
            {
                if(_hoverChild)
                {
                    _hoverChild->OnMouseLeave(e);
                }

                _hoverChild = child;
                Cursor::Set(child->Cursor.Get());
                child->OnMouseEnter(e);
                child->OnMouseMove(e);

                return;
            }
        }

        if(_hoverChild)
        {
            _hoverChild->OnMouseLeave(e);
            _hoverChild = nullptr;
        }

        Cursor::Set(Cursor.Get());
    }
}