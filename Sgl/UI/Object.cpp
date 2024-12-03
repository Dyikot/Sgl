#include "Object.h"
#include "../Application.h"

namespace Sgl
{
    Object::Object(SDL_FPoint position) noexcept:
        _position(position)
    {}

    Object::Object(SDL_FPoint position, const Style& style) noexcept:
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
        if(_isMouseOver && ToolTip)
        {
            ToolTip->OnRender(renderContext);
        }
    }

    void Object::OnMouseEnter(const MouseButtonEventArgs& e)
    {
        UIElement::OnMouseEnter(e);

        _isMouseOver = true;
        if(SDLCursor != nullptr)
        {
            SDL_SetCursor(SDLCursor);
        }
    }

    void Object::OnMouseLeave(const MouseButtonEventArgs& e)
    {
        UIElement::OnMouseLeave(e);
        _isMouseOver = false;
    }

    void Object::OnSizeChanged(IVisual* sender, const SizeChangedEventArgs& e)
    {
        if(SizeChanged)
        {
            SizeChanged(sender, e);
        }
    }
}