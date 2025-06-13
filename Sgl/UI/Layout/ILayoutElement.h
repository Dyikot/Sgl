#pragma once

#include "Layout.h"

namespace Sgl::UI
{
    class ILayoutElement
    {
    public:
        virtual ~ILayoutElement() = default;

        virtual void SetLayout(shared_ptr<Layout> value) = 0;
        virtual shared_ptr<Layout> GetLayout() const = 0;
    };
}