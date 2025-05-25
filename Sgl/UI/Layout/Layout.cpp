#include "Layout.h"

namespace Sgl::UI
{
    bool IsHover(FPoint point, FPoint position, float width, float height)
    {
        return point.x >= position.x &&
               point.x <= position.x + width &&
               point.y >= position.y &&
               point.y <= position.y + height;
    }

    Layout::Layout(VisualElement& parent):
        Parent(parent)
    {}

    void Layout::AddChild(UIElement& child)
    {
        _children.emplace(child);
    }

    void Layout::RemoveChild(UIElement& child)
    {
        _children.erase(child);
    }

    void Layout::OnMouseMove(const MouseButtonEventArgs& e)
    {
        if(_hoverChild && IsHover(e.Position, _hoverChild->_position,
                                              _hoverChild->Width,
                                              _hoverChild->Height))
        {
            _hoverChild->OnMouseMove(e);
            return;
        }

        for(UIElement& child : _children)
        {
            if(IsHover(e.Position, child._position, child.Width, child.Height))
            {
                if(_hoverChild)
                {
                    _hoverChild->OnMouseLeave(e);
                }

                _hoverChild = &child;
                Cursor::Set(child.Cursor);
                child.OnMouseEnter(e);
                child.OnMouseMove(e);

                return;
            }
        }

        if(_hoverChild)
        {
            _hoverChild->OnMouseLeave(e);
        }

        _hoverChild = nullptr;
        Cursor::Set(Parent.Cursor);
    }

    void Layout::OnRootLayoutMouseMove(const MouseButtonEventArgs& e)
    {
        bool wasHover = _isHover;
        _isHover = IsHover(e.Position, GetPosition(), Width, Height);

        if(wasHover)
        {
            if(_isHover)
            {
                base::OnMouseMove(e);
            }
            else
            {
                base::OnMouseLeave(e);
                return;
            }
        }
        else
        {
            if(_isHover)
            {
                base::OnMouseEnter(e);
                base::OnMouseMove(e);
            }
            else
            {
                return;
            }
        }

        OnMouseMove(e);
    }
}