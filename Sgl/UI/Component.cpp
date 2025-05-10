#include "Component.h"

namespace Sgl
{
    void Component::OnRender(RenderContext renderContext) const
    {
        if(_isHover && Style.Tooltip)
        {
            Style.Tooltip->OnRender(renderContext);
        }
    }

    void Component::OnMouseEnter(const MouseButtonEventArgs& e)
    {
        _isHover = true;
        SetStyle(_hoverStyleSetter);
        MouseEnter.TryRaise(*this, e);
    }

    void Component::OnMouseLeave(const MouseButtonEventArgs& e)
    {
        _isHover = false;
        SetClassStyle();
        MouseLeave.TryRaise(*this, e);
    }
}