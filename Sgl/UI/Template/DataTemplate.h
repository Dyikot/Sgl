#pragma once

#include <memory>
#include "../../Base/AnyView.h"

namespace Sgl
{
    using std::shared_ptr;
    class UIElement;

    class IDataTemplate
    {
    public:
        virtual ~IDataTemplate() = default;

        virtual shared_ptr<UIElement> Build(AnyConstView data) const = 0;
    };

    class UIElementDataTemplate: public IDataTemplate
    {
    public:
        static std::shared_ptr<UIElementDataTemplate> New()
        { 
            return std::make_shared<UIElementDataTemplate>();
        }

        shared_ptr<UIElement> Build(AnyConstView data) const override
        {
            return data.As<shared_ptr<UIElement>>();
        }
    };
}