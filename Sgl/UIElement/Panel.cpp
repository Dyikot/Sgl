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
        Children(*this),
        _currentChild()
    {}

    Panel::Panel(Panel&& other) noexcept:
        UIElement(std::move(other)),
        Children(std::move(Children)),
        _currentChild(std::exchange(other._currentChild, nullptr))
    {}

    void Panel::SetVisualRoot(IVisualRoot* value)
    {
        UIElement::SetVisualRoot(value);

        for(auto& child : Children)
        {
            child->SetVisualRoot(value);
        }
    }

    void Panel::SetStylingRoot(IStyleHost* value)
    {
        UIElement::SetStylingRoot(value);

        for(auto& child : Children)
        {
            child->SetStylingRoot(value);
        }
    }

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

    void Panel::OnChildAdded(UIElement& child)
    {
        child.SetParent(this);
        
        if(child.IsAttachedToLogicalTree())
        {
            child.OnAttachedToLogicalTree();
        }
    }

    void Panel::OnChildRemoving(UIElement& child)
    {
        if(child.IsAttachedToLogicalTree())
        {
            child.OnDetachedFromLogicalTree();
        }

        child.SetParent(nullptr);
    }    

    void Panel::OnAttachedToLogicalTree()
    {
        UIElement::OnAttachedToLogicalTree();

        for(auto& child : Children)
        {
            child->OnAttachedToLogicalTree();
        }
    }

    void Panel::OnDetachedFromLogicalTree()
    {
        UIElement::OnDetachedFromLogicalTree();

        for(auto& child : Children)
        {
            child->OnDetachedFromLogicalTree();
        }
    }

    void Panel::OnCursorChanged(const Cursor& cursor)
    {
        UIElement::OnCursorChanged(cursor);

        for(auto& child : Children)
        {
            child->SetCursor(cursor, ValueSource::Inheritance);
        }
    }

    void Panel::OnDataContextChanged(const Ref<INotifyPropertyChanged>& dataContext)
    {
        for(auto& child : Children)
        {
            child->SetDataContext(dataContext, ValueSource::Inheritance);
        }
    }

    void Panel::OnMouseMove(MouseMoveEventArgs e)
    {
        UIElement::OnMouseMove(e);

        auto& current = _currentChild;

        if(current)
        {   
            if(current->IsVisible() && LayoutHelper::IsPointInRect(e.X, e.Y, current->GetBounds()))
            {
                current->OnMouseMove(e);
                return;
            }
            else
            {
                current->OnMouseLeave(e);
                current = nullptr;
            }
        }

        for(auto& child : Children)
        {
            if(!child->IsVisible())
            {
                continue;
            }

            if(LayoutHelper::IsPointInRect(e.X, e.Y, child->GetBounds()))
            {
                Cursor::Set(child->GetCursor());

                current = child;
                child->OnMouseEnter(e);
                child->OnMouseMove(e);
                return;
            }
        }        

        Cursor::Set(GetCursor());
    }

    void Panel::OnMouseDown(MouseButtonEventArgs e)
    {
        UIElement::OnMouseDown(e);

        if(_currentChild && _currentChild->IsVisible() && _currentChild->IsMouseOver())
        {
            _currentChild->OnMouseDown(e);
        }
    }

    void Panel::OnMouseUp(MouseButtonEventArgs e)
    {
        UIElement::OnMouseUp(e);

        if(_currentChild && _currentChild->IsVisible() && _currentChild->IsMouseOver())
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