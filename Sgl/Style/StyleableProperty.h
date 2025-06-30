#pragma once

#include <type_traits>

namespace Sgl
{
    template<typename T, typename TValue = T>
        requires std::convertible_to<TValue, T> && std::copyable<T>
    class StylyableProperty
    {
    public:
        using Type = T;
        using ValueType = TValue;
    protected:
        T _value;
    public:
        StylyableProperty() requires std::default_initializable<T>:
            _value()
        {}

        StylyableProperty(TValue value):
            _value(value)
        {}

        StylyableProperty(const StylyableProperty& other): 
            _value(other._value)
        {}

        StylyableProperty(StylyableProperty&& other) noexcept:
            _value(std::move(other._value))
        {}

        virtual ~StylyableProperty() = default;

        virtual TValue Get() const { return _value; }
        operator TValue() const { return Get(); }

        virtual StylyableProperty& operator=(TValue value)
        {
            _value = value;
            return *this;
        }

        StylyableProperty& operator=(const StylyableProperty& other)
        {
            _value = other._value;
            return *this;
        }

        StylyableProperty& operator=(StylyableProperty&& other) noexcept
        {
            _value = std::move(other._value);
            return *this;
        }

        const T* operator->() const { return &_value; }
        T* operator->() { return &_value; }
    };
}