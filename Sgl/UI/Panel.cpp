#include "Panel.h"
#include "../Base/Math.h"

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

    void Panel::OnRender(RenderContext context) const
    {
        for(auto& child : Children)
        {
            if(child->IsVisible)
            {
                child->OnRender(context);
            }
        }

        UIElement::OnRender(context);
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
        UIElement::OnMouseMove(e);

        bool isCurrentVisible = _currentChild && _currentChild->IsVisible;
        if(isCurrentVisible && Math::IsPointInRect(e.Position, _currentChild->GetBounds()))
        {
            _currentChild->OnMouseMove(e);
            return;
        }

        for(auto& child : Children)
        {
            if(Math::IsPointInRect(e.Position, child->GetBounds()) && child->IsVisible)
            {
                if(_currentChild && _currentChild->IsVisible)
                {
                    _currentChild->OnMouseLeave(e);
                }

                _currentChild = child;
                Cursor::Set(child->Cursor);
                child->OnMouseEnter(e);
                child->OnMouseMove(e);

                return;
            }
        }

        if(_currentChild && _currentChild->IsVisible)
        {
            _currentChild->OnMouseLeave(e);
            _currentChild = nullptr;
        }    
    }

    void Panel::OnMouseDown(const MouseButtonEventArgs& e)
    {
        UIElement::OnMouseDown(e);

        if(_currentChild && _currentChild->IsMouseOver() && _currentChild->IsVisible)
        {
            _currentChild->OnMouseDown(e);
        }
    }

    void Panel::OnMouseUp(const MouseButtonEventArgs& e)
    {
        UIElement::OnMouseUp(e);

        if(_currentChild && _currentChild->IsMouseOver() && _currentChild->IsVisible)
        {
            _currentChild->OnMouseUp(e);
        }
    }

    void Panel::OnMouseLeave(const MouseEventArgs& e)
    {
        UIElement::OnMouseLeave(e);

        if(_currentChild && _currentChild->IsVisible)
        {
            _currentChild->OnMouseLeave(e);
            _currentChild = nullptr;
        }
    }
}