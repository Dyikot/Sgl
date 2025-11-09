#include "ValueConverters.h"

namespace Sgl::Converters
{
    std::string IntToStringConverter::operator()(int value) const
    {
        return std::to_string(value);
    }

    int IntToStringConverter::operator()(const std::string& value) const
    {
        return std::stoi(value);
    }
}