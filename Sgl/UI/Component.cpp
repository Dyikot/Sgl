#include "Component.h"

namespace Sgl
{
    void Component::OnRender(RenderContext renderContext) const
    {
        if(auto tooltip = style.tooltip; _hover && tooltip)
        {
            tooltip->OnRender(renderContext);
        }
    }

    void Component::OnMouseEnter(const MouseButtonEventArgs& e)
    {
        _hover = true;
        SetStyle(_hoverStyleSetter);
        mouseEnter.TryRaise(*this, e);
    }

    void Component::OnMouseLeave(const MouseButtonEventArgs& e)
    {
        _hover = false;
        SetClassStyle();
        mouseLeave.TryRaise(*this, e);
    }
}