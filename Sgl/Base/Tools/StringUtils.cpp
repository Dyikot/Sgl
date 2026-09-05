#include "StringUtils.h"
#include <ranges>

using namespace std::views;

namespace Sgl
{
    std::vector<std::string> SplitString(std::string_view str, char delimiter)
    {
        auto strings = str | split(delimiter) | transform([](auto&& range)
        {
            return std::string(range.begin(), range.end());
        });

        return { strings.begin(), strings.end() };
    }    
}

