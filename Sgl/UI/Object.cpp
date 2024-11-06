#include "Object.h"

namespace Sgl
{
    void Object::SetStyle(const Style& style)
    {
        UIElement::SetStyle(style);
        style.TryInit(SizeChangedProperty, SizeChanged);
        style.TryInit(WidthProperty, _width);
        style.TryInit(HeightProperty, _height);
        style.TryInit(MaxWidthProperty, _maxWidth);
        style.TryInit(MaxHeightProperty, _maxHeight);
        style.TryInit(MinWidthProperty, _minWidth);
        style.TryInit(MinHeightProperty, _minHeight);
        style.TryInit(MarginProperty, _margin);
        style.TryInit(CursorProperty, SDLCursor);
        style.TryInit(ToolTipProperty, ToolTip);
        style.TryInit(VerticalAligmentProperty, _verticalAlignment);
        style.TryInit(HorizontalAlignmentProperty, _horizontalAlignment);
    }

    void Object::OnRender() const
    {
        if(_isToolTipRenderRequired)
        {
            ToolTip->OnRender();
        }
    }

    bool Object::IsMouseOver(const SDL_Point& mousePosition) const noexcept
    {
        return mousePosition.x >= _position.x &&
               mousePosition.x <= _position.x + _width &&
               mousePosition.y >= _position.y &&
               mousePosition.y >= _position.y + _height;
    }

    void Object::OnMouseEnter(const MouseButtonEventArgs& e)
    {
        UIElement::OnMouseEnter(e);
        if(SDLCursor != nullptr)
        {
            SDL_SetCursor(SDLCursor);
        }
        _isToolTipRenderRequired = ToolTip;
    }

    void Object::OnMouseLeave(const MouseButtonEventArgs& e)
    {
        UIElement::OnMouseLeave(e);
        _isToolTipRenderRequired = false;
    }

    void Object::OnSizeChanged(IVisual* sender, const SizeChangedEventArgs& e)
    {
        if(SizeChanged)
        {
            SizeChanged(sender, e);
        }
    }
}