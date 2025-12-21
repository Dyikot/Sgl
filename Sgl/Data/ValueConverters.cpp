#include "ValueConverters.h"

namespace Sgl::Converters
{
    std::string Int32ToStringConverter::operator()(int value) const
    {
        return std::to_string(value);
    }

    int Int32ToStringConverter::operator()(const std::string& value) const
    {
        return std::stoi(value);
    }

    std::string Int64ToStringConverter::operator()(int64_t value) const
    {
        return std::to_string(value);
    }

    int64_t Int64ToStringConverter::operator()(const std::string& value) const
    {
        return std::stoll(value);
    }

    std::string UInt64ToStringConverter::operator()(uint64_t value) const
    {
        return std::to_string(value);
    }

    uint64_t UInt64ToStringConverter::operator()(const std::string& value) const
    {
        return std::stoull(value);
    }

    std::string UInt32ToStringConverter::operator()(uint32_t value) const
    {
        return std::to_string(value);
    }

    uint32_t UInt32ToStringConverter::operator()(const std::string& value) const
    {
        return std::stoul(value);
    }

    std::string Converters::FloatToStringConverter::operator()(float value) const
    {
        return std::to_string(value);
    }

    float Converters::FloatToStringConverter::operator()(const std::string& value) const
    {
        return std::stof(value);
    }

    std::string Converters::DoubleToStringConverter::operator()(double value) const
    {
        return std::to_string(value);
    }

    double Converters::DoubleToStringConverter::operator()(const std::string& value) const
    {
        return std::stof(value);
    }
}