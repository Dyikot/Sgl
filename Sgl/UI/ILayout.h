#pragma once

#include "../Style/StyleMap.h"

namespace Sgl
{
    class ILayout: public virtual IStyleProvider
    {
    public:
        virtual ~ILayout() = default;

        virtual void QueryArrange() = 0;
    protected:
        virtual void Arrange() = 0;
    };
}