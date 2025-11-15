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

    std::string LongToStringConverter::operator()(long value) const
    {
        return std::to_string(value);
    }

    long LongToStringConverter::operator()(const std::string& value) const
    {
        return std::stol(value);
    }

    std::string LongLongToStringConverter::operator()(long long value) const
    {
        return std::to_string(value);
    }

    long long LongLongToStringConverter::operator()(const std::string& value) const
    {
        return std::stoll(value);
    }

    std::string UnsignedLongLongToStringConverter::operator()(unsigned long long value) const
    {
        return std::to_string(value);
    }

    unsigned long long UnsignedLongLongToStringConverter::operator()(const std::string& value) const
    {
        return std::stoull(value);
    }

    std::string UnsignedIntToStringConverter::operator()(unsigned int value) const
    {
        return std::to_string(value);
    }

    unsigned int UnsignedIntToStringConverter::operator()(const std::string& value) const
    {
        return std::stoul(value);
    }

    std::string UnsignedLongToStringConverter::operator()(unsigned long value) const
    {
        return std::to_string(value);
    }

    unsigned long UnsignedLongToStringConverter::operator()(const std::string& value) const
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