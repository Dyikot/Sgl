#include "Component.h"

namespace Sgl
{
    Component::Component():
        Position(), 
        Children(*this)
    {
        Class.AddTarget(StyleTarget::Hover);
    }

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
        Class.ApplyStyleTo(StyleTarget::Hover);
        MouseEnter.TryRaise(*this, e);
    }

    void Component::OnMouseLeave(const MouseButtonEventArgs& e)
    {
        _isHover = false;
        Class.ApplyStyleTo(StyleTarget::Element);
        MouseLeave.TryRaise(*this, e);
    }
}