#include "Panel.h"
#include "../Layout/LayoutHelper.h"

namespace Sgl
{  
    Panel::Panel():
        UIElement(),
        Children(*this)
    {}

    Panel::Panel(const Panel& other):
        UIElement(other),
        Children(other.Children),
        _currentChild(other._currentChild)
    {}

    Panel::Panel(Panel&& other) noexcept:
        UIElement(std::move(other)),
        Children(std::move(Children)),
        _currentChild(std::exchange(other._currentChild, nullptr))
    {}

    void Panel::Render(RenderContext context)
    {
        RenderBackground(context);

        for(auto& child : Children)
        {
            if(child->IsVisible())
            {
                child->Render(context);
            }
        }

        UIElement::Render(context);
    }

    void Panel::ApplyStyle()
    {
        StyleableElement::ApplyStyle();

        for(auto& child : Children)
        {
            child->ApplyStyle();
        }
    }

    void Panel::OnCursorChanged(const Cursor& cursor)
    {
        if(IsMouseOver())
        {
            for(auto& child : Children)
            {
                if(child->IsMouseOver())
                {
                    return;
                }
            }

            Cursor::Set(cursor);
        }
    }

    void Panel::OnMouseMove(MouseMoveEventArgs e)
    {
        UIElement::OnMouseMove(e);

        bool isCurrentVisible = _currentChild && _currentChild->IsVisible();
        bool isMouseOverCurrent = LayoutHelper::IsPointInRect(e.X, e.Y, _currentChild->GetBounds());

        if(isCurrentVisible && isMouseOverCurrent)
        {
            _currentChild->OnMouseMove(e);
            return;
        }

        for(auto& child : Children)
        {
            bool isMouseOver = LayoutHelper::IsPointInRect(e.X, e.Y, _currentChild->GetBounds());

            if(isMouseOver && child->IsVisible())
            {
                if(_currentChild && _currentChild->IsVisible())
                {
                    _currentChild->OnMouseLeave(e);
                }

                _currentChild = child;
                Cursor::Set(child->GetCursor());
                child->OnMouseEnter(e);
                child->OnMouseMove(e);

                return;
            }
        }

        if(_currentChild && _currentChild->IsVisible())
        {
            _currentChild->OnMouseLeave(e);
            _currentChild = nullptr;
        }    
    }

    void Panel::OnMouseDown(MouseButtonEventArgs e)
    {
        UIElement::OnMouseDown(e);

        if(_currentChild && _currentChild->IsMouseOver() && _currentChild->IsVisible())
        {
            _currentChild->OnMouseDown(e);
        }
    }

    void Panel::OnMouseUp(MouseButtonEventArgs e)
    {
        UIElement::OnMouseUp(e);

        if(_currentChild && _currentChild->IsMouseOver() && _currentChild->IsVisible())
        {
            _currentChild->OnMouseUp(e);
        }
    }

    void Panel::OnMouseLeave(MouseMoveEventArgs e)
    {
        UIElement::OnMouseLeave(e);

        if(_currentChild && _currentChild->IsVisible())
        {
            _currentChild->OnMouseLeave(e);
            _currentChild = nullptr;
        }
    }
}