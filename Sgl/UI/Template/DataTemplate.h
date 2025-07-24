#pragma once

#include <memory>
#include "../../Base/Any.h"

namespace Sgl
{
    class UIElement;

    class IDataTemplate
    {
    public:
        virtual ~IDataTemplate() = default;

        virtual Shared<UIElement> Build(const Any& data) const = 0;
    };

    class UIElementDataTemplate: public IDataTemplate
    {
    public:
        Shared<UIElement> Build(const Any& data) const override
        {
            return data.As<Shared<UIElement>>();
        }
    };
}