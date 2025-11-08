#pragma once

#include "../Base/Ref.h"
#include "../Base/Delegate.h"

namespace Sgl
{
    class IData;
    class UIElement;

    using DataTemplate = Delegate<Ref<UIElement>(const Ref<IData>&)>;

    class IData
    {
    public:
        virtual ~IData() = default;

        virtual DataTemplate GetDefaultDataTemplate() const = 0;
    };
}