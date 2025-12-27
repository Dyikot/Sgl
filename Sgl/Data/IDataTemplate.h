#pragma once

#include "../Base/Ref.h"

namespace Sgl
{
    class UIElement;
    class INotityPropertyChanged;

    class IDataTemplate
    {
    public:
        virtual ~IDataTemplate() = default;

        virtual Ref<UIElement> Build(const Ref<INotityPropertyChanged>& data) = 0;
        virtual bool Match(const Ref<INotityPropertyChanged>& data) const = 0;
    };
}