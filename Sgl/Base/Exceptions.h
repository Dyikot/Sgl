#pragma once

#include <format>
#include <exception>

namespace Sgl
{
    //! @brief An exception class that supports formatted error messages.
    //! Provides a constructor that accepts a format string and arguments (using C++20 std::format),
    //! enabling clear, context-rich error reporting.
    class Exception : public std::exception
    {
    public:
        //! @brief Constructs an exception with a pre-formatted message
        explicit Exception(std::string_view message):
            _message(message.data())
        {}

        //! @brief Constructs an exception by formatting a message using std::format
        template<typename... TArgs>
        Exception(std::format_string<TArgs...> format, TArgs&&... args):
            _message(std::format(format, std::forward<TArgs>(args)...))
        {}

        //! @brief Returns the formatted error message
        const char* what() const noexcept override { return _message.c_str(); }
    private:
        std::string _message;
    };
}