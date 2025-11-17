#pragma once

#include "../Base/Ref.h"
#include "../Base/Delegate.h"

namespace Sgl
{
    class IData;
    class UIElement;

    class IDataTemplate
    {
    public:
        virtual ~IDataTemplate() = default;

        virtual Ref<UIElement> Build(const Ref<IData>& data) const = 0;
        virtual bool Match(const Ref<IData>& data) const = 0;
    };
}