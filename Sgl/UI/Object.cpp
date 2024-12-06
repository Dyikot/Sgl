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
        style.TryCopyTo(WidthProperty, _width);
        style.TryCopyTo(HeightProperty, _height);
        style.TryCopyTo(MaxWidthProperty, _maxWidth);
        style.TryCopyTo(MaxHeightProperty, _maxHeight);
        style.TryCopyTo(MinWidthProperty, _minWidth);
        style.TryCopyTo(MinHeightProperty, _minHeight);
        style.TryCopyTo(MarginProperty, _margin);
        style.TryCopyTo(CursorProperty, ObjectCursor);
        style.TryCopyTo(ToolTipProperty, ToolTip);
        style.TryCopyTo(VerticalAligmentProperty, _verticalAlignment);
        style.TryCopyTo(HorizontalAlignmentProperty, _horizontalAlignment);
        style.TryAddHandlerToEvent(MouseEnterProperty, MouseEnter);
        style.TryAddHandlerToEvent(MouseLeaveProperty, MouseLeave);
        style.TryAddHandlerToEvent(SizeChangedProperty, SizeChanged);
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
        _isMouseOver = true;

        if(ObjectCursor)
        {
            Application::Current()->SetCursor(*ObjectCursor);
        }

        if(MouseEnter)
        {
            MouseEnter(this, e);
        }        
    }

    void Object::OnMouseLeave(const MouseButtonEventArgs& e)
    {
        _isMouseOver = false;

        if(ObjectCursor)
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