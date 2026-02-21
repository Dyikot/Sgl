#include "Converters.h"

namespace Sgl::Converters
{
    std::string IntToStringConverter::Convert(int value) const
    {
        return std::to_string(value);
    }

    int IntToStringConverter::ConvertBack(const std::string& value) const
    {
        return std::stoi(value);
    }
}

