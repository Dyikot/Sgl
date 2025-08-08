#pragma once

#include "DataTemplate.h"
#include "../UIElements/TextBlock.h"

namespace Sgl
{
    class StringDataTemplate: public IDataTemplate
    {
    public:
        Shared<UIElement> Build(const Any& data) const override
        {
            const auto& text = data.As<std::string>();
            auto textBlock = NewShared<UIElements::TextBlock>();
            textBlock->SetText(text);
            return textBlock;
        }
    };
}