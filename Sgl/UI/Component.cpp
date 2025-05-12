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
}