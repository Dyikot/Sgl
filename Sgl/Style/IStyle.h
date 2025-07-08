#pragma once

namespace Sgl
{
    class StyleableElement;

    class IStyle
    {
    public:
        virtual ~IStyle() = default;

        virtual void Apply(StyleableElement& target) = 0;
    };
}