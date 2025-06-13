#pragma once

#include <string>
#include "../Visual/Font.h"

namespace Sgl::UI
{
    class ITextElement
    {
    public:
        virtual ~ITextElement() = default;

        virtual void SetText(std::string value) = 0;
        virtual std::string GetText() const = 0;

        virtual void SetFont(std::shared_ptr<Font> value) = 0;
        virtual std::shared_ptr<Font> GetFont() const = 0;
    };
}