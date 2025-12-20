#pragma once

#include "StyleCollection.h"

namespace Sgl
{
    class IStyleHost
    {
    public:
        virtual ~IStyleHost() = default;

        virtual StyleCollection& GetStyles() = 0;
        virtual IStyleHost* GetStylingParent() = 0;
        virtual IStyleHost* GetStylingRoot() = 0;
    };
}