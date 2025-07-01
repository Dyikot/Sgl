#pragma once

#include <type_traits>

namespace Sgl
{
    template<typename T, typename TInput = T>
        requires std::convertible_to<TInput, T> && std::copyable<T>
    class StylyableProperty
    {
    public:
        using Type = T;
        using InputType = TInput;
    protected:
        T _value;
    public:
        StylyableProperty() requires std::default_initializable<T>:
            _value()
        {}

        StylyableProperty(TInput value):
            _value(value)
        {}

        StylyableProperty(const StylyableProperty& other): 
            _value(other._value)
        {}

        StylyableProperty(StylyableProperty&& other) noexcept:
            _value(std::move(other._value))
        {}

        virtual ~StylyableProperty() = default;

        virtual TInput Get() const { return _value; }
        operator TInput() const { return Get(); }

        virtual StylyableProperty& operator=(TInput value)
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