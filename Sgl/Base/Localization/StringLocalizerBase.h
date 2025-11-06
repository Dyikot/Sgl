#pragma once

#include <string>
#include <string_view>
#include <format>

namespace Sgl
{
    class StringLocalizerBase
    {
    public:
        virtual ~StringLocalizerBase() = default;

        virtual void SetCulture(const std::string& culture) = 0;
        virtual const std::string& GetCulture() const = 0;

        virtual const std::string& operator()(const std::string& name) const = 0;

        template<typename... TArgs>
        std::string operator()(const std::string& name, TArgs&&... args) const
        {
            return std::vformat(operator()(name), std::make_format_args(args...));
        }
    };
}