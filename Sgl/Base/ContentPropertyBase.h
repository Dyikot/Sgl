#pragma once

#include "Any.h"

namespace Sgl
{
    class ContentPropertyBase
    {
    protected:
        Any _value;
    public:
        ContentPropertyBase():
            _value()
        {}

        ContentPropertyBase(const Any& value):
            _value(value)
        {}

        ContentPropertyBase(const ContentPropertyBase& other):
            _value(other._value)
        {}

        ContentPropertyBase(ContentPropertyBase&& other) noexcept:
            _value(std::move(other._value))
        {}

        const Any& Get() const { return _value; }

        ContentPropertyBase& operator=(const ContentPropertyBase& other)
        {
            if(this != &other)
            {
                _value = other._value;
            }

            return *this;
        }

        ContentPropertyBase& operator=(ContentPropertyBase&& other) noexcept
        {
            _value = std::move(other._value);
            return *this;
        }
    };
}