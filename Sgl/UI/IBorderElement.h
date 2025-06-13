#pragma once
#include "../Style/Layout.h"

namespace Sgl::UI
{
    class IBorderElement
    {
    public:
        virtual ~IBorderElement() = default;

        virtual void SetPadding(const Thickness& value) = 0;
        virtual const Thickness& GetPadding() const = 0;

        virtual void SetBorderThickness(const Thickness& value) = 0;
        virtual const Thickness& GetBorderThickness() const = 0;

        virtual void SetBorderColor(Color value) = 0;
        virtual Color GetBorderColor() const = 0;
    };
}