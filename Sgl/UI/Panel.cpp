#include "Panel.h"

namespace Sgl
{  
    Panel::Panel():
        UIElement(),
        Children(*this)
    {}

    Panel::Panel(const Panel& other):
        UIElement(other),
        Children(other.Children)
    {}

    Panel::Panel(Panel&& other) noexcept:
        UIElement(std::move(other)),
        Children(std::move(Children))
    {}

    static bool IsPointInBounds(FPoint point, FRect bounds)
    {
        return point.x >= bounds.x &&
               point.x <= bounds.x + bounds.w &&
               point.y >= bounds.y &&
               point.y <= bounds.y + bounds.h;
    }

    void Sgl::Panel::OnRender(RenderContext context) const
    {
        UIElement::OnRender(context);
        
        for(auto& child : Children)
        {
            child->OnRender(context);
        }
    }

    void Panel::ApplyStyle()
    {
        StyleableElement::ApplyStyle();

        for(auto& child : Children)
        {
            child->ApplyStyle();
        }
    }

    void Panel::OnMouseMove(const MouseEventArgs& e)
    {
        bool wasMouseOver = _isMouseOver;
        _isMouseOver = IsPointInBounds(e.Position, GetBounds());

        if(_isMouseOver)
        {
            if(!wasMouseOver)
            {
                OnMouseEnter(e);
            }

            OnChildrenMouseMove(e);
        }
        else if(wasMouseOver)
        {
            OnMouseLeave(e);
        }
    }

    void Panel::OnMouseDown(const MouseButtonEventArgs& e)
    {
        UIElement::OnMouseDown(e);

        if(_mouseOverChild)
        {
            _mouseOverChild->OnMouseDown(e);
        }
    }

    void Panel::OnMouseUp(const MouseButtonEventArgs& e)
    {
        UIElement::OnMouseUp(e);

        if(_mouseOverChild)
        {
            _mouseOverChild->OnMouseUp(e);
        }
    }

    void Panel::OnMouseWheelChanged(const MouseWheelEventArgs& e)
    {
        for(auto& child : Children)
        {
            child->OnMouseWheelChanged(e);
        }
    }

    void Panel::OnKeyDown(const KeyEventArgs& e)
    {
        for(auto& child : Children)
        {
            child->OnKeyDown(e);
        }
    }

    void Panel::OnKeyUp(const KeyEventArgs& e)
    {
        for(auto& child : Children)
        {
            child->OnKeyUp(e);
        }
    }

    void Panel::OnChildrenMouseMove(const MouseEventArgs& e)
    {
        if(_mouseOverChild && IsPointInBounds(e.Position, _mouseOverChild->GetBounds()))
        {
            _mouseOverChild->OnMouseMove(e);
            return;
        }

        for(auto& child : Children)
        {
            if(IsPointInBounds(e.Position, child->GetBounds()))
            {
                if(_mouseOverChild)
                {
                    _mouseOverChild->OnMouseLeave(e);
                    _mouseOverChild->_isMouseOver = false;
                }

                _mouseOverChild = child;
                _mouseOverChild->_isMouseOver = true;
                Cursor::Set(child->Cursor);
                child->OnMouseEnter(e);
                child->OnMouseMove(e);

                return;
            }
        }

        if(_mouseOverChild)
        {
            _mouseOverChild->OnMouseLeave(e);
            _mouseOverChild->_isMouseOver = false;
            _mouseOverChild = nullptr;
        }

        Cursor::Set(Cursor);
    }
}