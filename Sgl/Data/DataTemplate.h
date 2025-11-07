#pragma once

#include "../Base/Ref.h"
#include "../Base/Delegate.h"

namespace Sgl
{
    class UIElement;
    
    class IData
    {
    public:
        virtual ~IData() = default;
    };

    using DataTemplate = Delegate<Ref<UIElement>(const Ref<IData>&)>;
}