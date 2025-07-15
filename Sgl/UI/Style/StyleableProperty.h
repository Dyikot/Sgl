#pragma once

#include <concepts>

namespace Sgl
{
    template<typename T, typename TInput = T> 
        requires std::constructible_from<T, TInput> && std::copyable<T>
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

        void Set(TInput value)
        {
            _value = value;
            OnChanged();
        }

        TInput Get() const { return _value; }

        virtual void OnChanged(){}

        operator TInput() const { return _value; }

        StylyableProperty& operator=(TInput value)
        {
            Set(value);
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
    };
}