#include "Converters.h"
#include "../Base/Logging.h"

namespace Sgl::Converters
{
    std::string IntToStringConverter::operator()(int value) const
    {
        return std::format("{}", value);
    }

    int IntToStringConverter::operator()(const std::string& value) const
    {
        try
        {
            return std::stoi(value);
        }
        catch (const std::exception& e)
        {
            Logging::LogError("IntToStringConverter: Failed to convert '{}' - {}", value, e.what());
            return Fallback;
        }
    }

    std::string UIntToStringConverter::operator()(uint32_t value) const
    {
        return std::format("{}", value);
    }

    uint32_t UIntToStringConverter::operator()(const std::string& value) const
    {
        try
        {
            return static_cast<uint32_t>(std::stoul(value));
        }
        catch (const std::exception& e)
        {
            Logging::LogError("UIntToStringConverter: Failed to convert '{}' - {}", value, e.what());
            return Fallback;
        }
    }

    std::string Int64ToStringConverter::operator()(int64_t value) const
    {
        return std::format("{}", value);
    }

    int64_t Int64ToStringConverter::operator()(const std::string& value) const
    {
        try
        {
            return std::stoll(value);
        }
        catch (const std::exception& e)
        {
            Logging::LogError("Int64ToStringConverter: Failed to convert '{}' - {}", value, e.what());
            return Fallback;
        }
    }

    std::string UInt64ToStringConverter::operator()(uint64_t value) const
    {
        return std::format("{}", value);
    }

    uint64_t UInt64ToStringConverter::operator()(const std::string& value) const
    {
        try
        {
            return std::stoull(value);
        }
        catch (const std::exception& e)
        {
            Logging::LogError("UInt64ToStringConverter: Failed to convert '{}' - {}", value, e.what());
            return Fallback;
        }
    }

    std::string FloatToStringConverter::operator()(float value) const
    {
        return std::format("{}", value);
    }

    float FloatToStringConverter::operator()(const std::string& value) const
    {
        try
        {
            return std::stof(value);
        }
        catch (const std::exception& e)
        {
            Logging::LogError("FloatToStringConverter: Failed to convert '{}' - {}", value, e.what());
            return Fallback;
        }
    }

    std::string DoubleToStringConverter::operator()(double value) const
    {
        return std::format("{}", value);
    }

    double DoubleToStringConverter::operator()(const std::string& value) const
    {
        try
        {
            return std::stod(value);
        }
        catch (const std::exception& e)
        {
            Logging::LogError("DoubleToStringConverter: Failed to convert '{}' - {}", value, e.what());
            return Fallback;
        }
    }

    bool InverseBoolConverter::operator()(bool value) const
    {
        return !value;
    }
}
