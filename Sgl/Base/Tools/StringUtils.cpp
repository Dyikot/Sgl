#include "StringUtils.h"
#include "../Views.h"

using namespace std::views;

namespace Sgl
{
    std::vector<std::string> SplitString(std::string_view str, char delimiter)
    {
        auto strings = str | split(delimiter) | Views::Cast<std::string>();
        return { strings.begin(), strings.end() };
    }    
}

