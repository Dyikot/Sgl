#include "Panel.h"
#include "../Base/Math.h"

namespace Sgl
{  
    Panel::Panel():
        UIElement(),
        Children(*this),
        _currentChild()
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

    void Panel::OnMouseMove(const MouseEventArgs& e)
    {
        UIElement::OnMouseMove(e);

        bool isCurrentVisible = _currentChild && _currentChild->IsVisible();
        if(isCurrentVisible && Math::IsPointInRect(e.Position, _currentChild->GetBounds()))
        {
            _currentChild->OnMouseMove(e);
            return;
        }

        for(auto& child : Children)
        {
            auto& element = child.GetValue();

            if(Math::IsPointInRect(e.Position, element.GetBounds()) && element.IsVisible())
            {
                if(_currentChild && _currentChild->IsVisible())
                {
                    _currentChild->OnMouseLeave(e);
                }

                _currentChild = child;
                Cursor::Set(element.GetCursor());
                element.OnMouseEnter(e);
                element.OnMouseMove(e);

                return;
            }
        }

        if(_currentChild && _currentChild->IsVisible())
        {
            _currentChild->OnMouseLeave(e);
            _currentChild = nullptr;
        }    
    }

    void Panel::OnMouseDown(const MouseButtonEventArgs& e)
    {
        UIElement::OnMouseDown(e);

        if(_currentChild && _currentChild->IsMouseOver() && _currentChild->IsVisible())
        {
            _currentChild->OnMouseDown(e);
        }
    }

    void Panel::OnMouseUp(const MouseButtonEventArgs& e)
    {
        UIElement::OnMouseUp(e);

        if(_currentChild && _currentChild->IsMouseOver() && _currentChild->IsVisible())
        {
            _currentChild->OnMouseUp(e);
        }
    }

    void Panel::OnMouseLeave(const MouseEventArgs& e)
    {
        UIElement::OnMouseLeave(e);

        if(_currentChild && _currentChild->IsVisible())
        {
            _currentChild->OnMouseLeave(e);
            _currentChild = nullptr;
        }
    }
}