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

        virtual std::shared_ptr<UIElement> Build(const Any& data) const = 0;
    };

    class UIElementDataTemplate: public IDataTemplate
    {
    public:
        static std::shared_ptr<UIElementDataTemplate> New()
        { 
            return std::make_shared<UIElementDataTemplate>();
        }

        std::shared_ptr<UIElement> Build(const Any& data) const override
        {
            return data.As<std::shared_ptr<UIElement>>();
        }
    };
}