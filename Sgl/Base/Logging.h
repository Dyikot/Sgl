#pragma once

#include <format>
#include <iostream>
#include <string_view>

namespace Sgl
{
    /// <summary>
    /// A simple, logging utility that supports formatted, color-coded output to stdout.
    /// Provides static methods for logging messages at different severity levels (Debug, Info, Warning, Error)
    /// with ANSI color codes for terminal readability. All logging is synchronous and uses std::format-style syntax.
    /// </summary>
    class Logging
    {
    public:
        /// <summary>
        /// Logs a raw formatted message without any prefix or color styling.
        /// </summary>
        /// <param name="format"> - a format string compatible with std::format.</param>
        /// <param name="args"> - arguments to substitute into the format string.</param>
        template<typename... TArgs>
        static void Log(std::string_view format, TArgs&&... args)
        {
            std::cout << std::vformat(format, std::make_format_args(args...));
        }

        /// <summary>
        /// Logs a debug-level message with cyan color and "[DEBUG]" prefix.
        /// </summary>
        /// <param name="format"> - a format string compatible with std::format.</param>
        /// <param name="args"> - arguments to substitute into the format string.</param>
        template<typename... TArgs>
        static void LogDebug(std::string_view format, TArgs&&... args)
        {
            Log(Format, DebugColor, "DEBUG", DefaultColor, std::vformat(format, std::make_format_args(args...)));
        }

        /// <summary>
        /// Logs an informational message with green color and "[INFO]" prefix.
        /// </summary>
        /// <param name="format"> - a format string compatible with std::format.</param>
        /// <param name="args"> - arguments to substitute into the format string.</param>
        template<typename... TArgs>
        static void LogInfo(std::string_view format, TArgs&&... args)
        {
            Log(Format, InfoColor, "INFO", DefaultColor, std::vformat(format, std::make_format_args(args...)));
        }

        /// <summary>
        /// Logs a warning message with yellow color and "[WARNING]" prefix.
        /// </summary>
        /// <param name="format"> - a format string compatible with std::format.</param>
        /// <param name="args"> - arguments to substitute into the format string.</param>
        template<typename... TArgs>
        static void LogWarning(std::string_view format, TArgs&&... args)
        {
            Log(Format, WarningColor, "WARNING", DefaultColor, std::vformat(format, std::make_format_args(args...)));
        }

        /// <summary>
        /// Logs an error message with red color and "[ERROR]" prefix.
        /// </summary>
        /// <param name="format"> - a format string compatible with std::format.</param>
        /// <param name="args"> - arguments to substitute into the format string.</param>
        template<typename... TArgs>
        static void LogError(std::string_view format, TArgs&&... args)
        {
            Log(Format, ErrorColor, "ERROR", DefaultColor, std::vformat(format, std::make_format_args(args...)));
        }
    private:
        inline static constexpr std::string_view Format = "{}[{}]{} {}\n";
        inline static constexpr std::string_view DefaultColor = "\033[0m";
        inline static constexpr std::string_view DebugColor = "\033[36m";
        inline static constexpr std::string_view InfoColor = "\033[32m";
        inline static constexpr std::string_view WarningColor = "\033[33m";
        inline static constexpr std::string_view ErrorColor = "\033[31m";
    };
}