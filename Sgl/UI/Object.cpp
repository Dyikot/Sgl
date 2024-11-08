#include "Object.h"

namespace Sgl
{
    Object::Object(const SDL_FPoint& position) noexcept:
        _position(position)
    {}

    Object::Object(const SDL_FPoint& position, const Style& style) noexcept:
        _position(position),
        UIElement(style)
    {
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

    Object::Object(const Style& style) noexcept:
        Object(SDL_FPoint{}, style)
    {}

    void Object::OnRender(RenderContext& renderContext) const
    {
        if(_isToolTipRenderRequired)
        {
            ToolTip->OnRender(renderContext);
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

        _isToolTipRenderRequired = ToolTip;
        if(SDLCursor != nullptr)
        {
            SDL_SetCursor(SDLCursor);
        }
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