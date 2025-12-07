#pragma once

#include "StyleMap.h"

namespace Sgl
{
    class IStyleHost
    {
    public:
        virtual ~IStyleHost() = default;

        virtual StyleMap& GetStyles() = 0;
        virtual IStyleHost* GetStylingParent() = 0;
    };
}