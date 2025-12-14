#pragma once

#include <format>
#include <iostream>
#include <string_view>

class Logger
{
private:
    inline static const std::string_view DefaultColor   = "\033[0m";
    inline static const std::string_view DebugColor     = "\033[36m";  
    inline static const std::string_view InfoColor      = "\033[32m";   
    inline static const std::string_view WarningColor   = "\033[33m"; 
    inline static const std::string_view ErrorColor     = "\033[31m";   

    template<typename... TArgs>
    static void Log(std::string_view levelName, 
                    std::string_view colorCode,
                    std::string_view format, 
                    TArgs&&... args)
    {
        auto message = std::vformat(format, std::make_format_args(args...));
        std::cout << std::format("{}[{}]{} {}\n", colorCode, levelName, DefaultColor, message);
    }
public:
    template<typename... TArgs>
    static void LogDebug(std::string_view format, TArgs&&... args)
    {
        Log("DEBUG", DebugColor, format, std::forward<TArgs>(args)...);
    }

    template<typename... TArgs>
    static void LogInfo(std::string_view format, TArgs&&... args)
    {
        Log("INFO", InfoColor, format, std::forward<TArgs>(args)...);
    }

    template<typename... TArgs>
    static void LogWarning(std::string_view format, TArgs&&... args)
    {
        Log("WARNING", WarningColor, format, std::forward<TArgs>(args)...);
    }

    template<typename... TArgs>
    static void LogError(std::string_view format, TArgs&&... args)
    {
        Log("ERROR", ErrorColor, format, std::forward<TArgs>(args)...);
    }
};