#pragma once

#include "../Base/Ref.h"
#include "../Base/Delegate.h"

namespace Sgl
{
    class ObservableObject;
    class UIElement;

    class IDataTemplate
    {
    public:
        virtual ~IDataTemplate() = default;

        virtual Ref<UIElement> Build(const Ref<ObservableObject>& data) = 0;
        virtual bool Match(const Ref<ObservableObject>& data) const = 0;
    };
}