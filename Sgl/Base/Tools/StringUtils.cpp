#include "StringUtils.h"

#include <ranges>

using namespace std::views;

namespace Sgl
{
    struct ToStringFn
    {
        std::string operator()(auto&& range) const
        {
            return std::string(range.begin(), range.end());
        }
    };

    static inline constexpr ToStringFn ToString;

    std::vector<std::string> SplitString(std::string_view str, char delimiter)
    {
        auto strings = str | split(delimiter) | transform(ToString);
        return std::vector<std::string>(strings.begin(), strings.end());
    }    
}

