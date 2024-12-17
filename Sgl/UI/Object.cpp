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
        style.TryInit(WidthProperty, _width);
        style.TryInit(HeightProperty, _height);
        style.TryInit(MaxWidthProperty, _maxWidth);
        style.TryInit(MaxHeightProperty, _maxHeight);
        style.TryInit(MinWidthProperty, _minWidth);
        style.TryInit(MinHeightProperty, _minHeight);
        style.TryInit(MarginProperty, _margin);
        style.TryInit(CursorProperty, Cursor);
        style.TryInit(ToolTipProperty, ToolTip);
        style.TryInit(VerticalAligmentProperty, _verticalAlignment);
        style.TryInit(HorizontalAlignmentProperty, _horizontalAlignment);
        style.TryInitEvent(MouseEnterProperty, MouseEnter);
        style.TryInitEvent(MouseLeaveProperty, MouseLeave);
        style.TryInitEvent(SizeChangedProperty, SizeChanged);
    }

    Object::Object(const Style& style) noexcept:
        Object(SDL_FPoint{}, style)
    {}

    void Object::OnRender(RenderContext& renderContext)
    {
        if(_isMouseOver && ToolTip)
        {
            ToolTip->OnRender(renderContext);
        }
    }

    void Object::OnMouseEnter(const MouseButtonEventArgs& e)
    {
        _isMouseOver = true;

        if(Cursor)
        {
            Application::Current()->SetCursor(*Cursor);
        }

        if(MouseEnter)
        {
            MouseEnter(this, e);
        }        
    }

    void Object::OnMouseLeave(const MouseButtonEventArgs& e)
    {
        _isMouseOver = false;

        if(Cursor)
        {
            Application::Current()->SetApplicationCursor();
        }

        if(MouseLeave)
        {
            MouseLeave(this, e);
        }
    }

    void Object::OnSizeChanged(IVisual* sender, const SizeChangedEventArgs& e)
    {
        if(SizeChanged)
        {
            SizeChanged(sender, e);
        }
    }
}