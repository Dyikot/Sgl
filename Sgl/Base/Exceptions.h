#pragma once

#include <format>
#include <execution>

namespace Sgl
{
    /// <summary>
    /// An exception class that supports formatted error messages.
    /// Provides a constructor that accepts a format string and arguments (using C++20 std::format),
    /// enabling clear, context-rich error reporting.
    /// </summary>
    class Exception: public std::exception
    {
    public:
        /// <summary>
        /// Constructs an exception with a pre-formatted message.
        /// </summary>
        /// <param name="message"> - the error message.</param>
        Exception(std::string_view message):
            _message(message.data())
        {}

        /// <summary>
        /// Constructs an exception by formatting a message using std::format.
        /// </summary>
        /// <param name="format"> - a format string (e.g., "File {} not found").</param>
        /// <param name="args"> - arguments to substitute into the format string.</param>
        template<typename... TArgs>
        Exception(std::format_string<TArgs...> format, TArgs&&... args):
            _message(std::format(format, std::forward<TArgs>(args)...))
        {}

        /// <summary>
        /// Returns the formatted error message.
        /// </summary>
        /// <returns>A null-terminated C-style string containing the error message.</returns>
        const char* what() const noexcept override { return _message.c_str(); }
    private:
        std::string _message;
    };
}