#include "Layout.h"

namespace Sgl::UI
{  
    Layout::Layout(VisualElement& parent):
        Parent(parent),
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

    struct OnUIElementSizeChanged
    {
        Layout& Layout;

        OnUIElementSizeChanged(UI::Layout& layout): Layout(layout) {}

        void operator()(UIElement& element) const
        {
            Layout.QueryArrange();
        }
    };

    void Layout::QueryArrange()
    {
        _shouldArrange = true;
    }

    void Layout::OnRender(RenderContext rc) const
    {
        base::OnRender(rc);
        
        for(auto& child : _children)
        {
            child->OnRender(rc);
        }
    }

    void Layout::Add(const std::shared_ptr<UIElement>& element)
    {
        _children.Add(element);
        element->SizeChanged += OnUIElementSizeChanged(*this);
        QueryArrange();
    }

    void Layout::Remove(const std::shared_ptr<UIElement>& element)
    {
        _children.Remove(element);
        element->SizeChanged -= OnUIElementSizeChanged(*this);
        QueryArrange();
    }

    void Layout::OnMouseMove(const MouseButtonEventArgs& e)
    {
        if(_hoverChild && IsHover(e.Position, _hoverChild->GetActualPosition(),
                                              _hoverChild->GetActualWidth(),
                                              _hoverChild->GetActualHeight()))
        {
            _hoverChild->OnMouseMove(e);
            return;
        }

        for(auto& child : _children)
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
                Cursor::Set(child->GetCursor());
                child->OnMouseEnter(e);
                child->OnMouseMove(e);

                return;
            }
        }

        if(_hoverChild)
        {
            _hoverChild->OnMouseLeave(e);
        }

        _hoverChild = nullptr;
        Cursor::Set(Parent.GetCursor());
    }

    void Layout::OnMouseDown(const MouseButtonEventArgs& e)
    {
        base::OnMouseDown(e);

        if(_hoverChild)
        {
            _hoverChild->OnMouseDown(e);
        }
    }

    void Layout::OnMouseUp(const MouseButtonEventArgs& e)
    {
        base::OnMouseUp(e);

        if(_hoverChild)
        {
            _hoverChild->OnMouseUp(e);
        }
    }

    void Layout::OnMouseWheel(const MouseWheelEventArgs& e)
    {
        for(auto& child : _children)
        {
            child->OnMouseWheel(e);
        }
    }

    void Layout::OnKeyDown(const KeyEventArgs& e)
    {
        for(auto& child : _children)
        {
            child->OnKeyDown(e);
        }
    }

    void Layout::OnKeyUp(const KeyEventArgs& e)
    {
        for(auto& child : _children)
        {
            child->OnKeyUp(e);
        }
    }

    void Layout::OnSizeChanged()
    {
        base::OnSizeChanged();

        if(_children.Count() > 0)
        {
            QueryArrange();
        }
    }

    void Layout::OnSceneLayoutMouseMove(const MouseButtonEventArgs& e)
    {
        bool wasHover = _isHover;
        _isHover = IsHover(e.Position, GetActualPosition(), GetActualWidth(), GetActualHeight());

        if(_isHover)
        {
            if(!wasHover)
            {
                OnMouseEnter(e);
            }

            OnMouseMove(e);
        }
        else if(wasHover)
        {
            OnMouseLeave(e);
        }
    }
}