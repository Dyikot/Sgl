#include "Component.h"

namespace Sgl
{
    void Component::SetHoverStyle(std::vector<StyleSelector> selectors)
    {
        _hoverSelectors = std::move(selectors);
    }

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
        ApplyStyle(_hoverSelectors);
        mouseEnter.TryRaise(*this, e);
    }

    void Component::OnMouseLeave(const MouseButtonEventArgs& e)
    {
        _hover = false;
        ApplyClassStyle();
        mouseLeave.TryRaise(*this, e);
    }
}