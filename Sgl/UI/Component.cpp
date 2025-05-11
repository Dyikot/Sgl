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
        ApplyStyle(_hoverStyleSelector);
        MouseEnter.TryRaise(*this, e);
    }

    void Component::OnMouseLeave(const MouseButtonEventArgs& e)
    {
        _isHover = false;
        ApplyDefaultStyle();
        MouseLeave.TryRaise(*this, e);
    }
}