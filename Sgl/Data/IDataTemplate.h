#pragma once

#include "../Base/Ref.h"

namespace Sgl
{
    class UIElement;
    class INotifyPropertyChanged;

    class IDataTemplate
    {
    public:
        virtual ~IDataTemplate() = default;

        virtual Ref<UIElement> Build(const Ref<INotifyPropertyChanged>& data) = 0;
        virtual bool Match(const Ref<INotifyPropertyChanged>& data) const = 0;
    };
}