#pragma once

#include "IObserver.h"
#include <concepts>

namespace Sgl
{
    template<typename T, typename TInput = T>
        requires std::constructible_from<T, TInput>&& std::copyable<T>
    class Observer: public IObserver<TInput>
    {
    protected:
        T _value;
    public:
        Observer() requires std::default_initializable<T> :
            _value()
        {}

        Observer(TInput value):
            _value(value)
        {}

        Observer(const Observer& other):
            _value(other._value)
        {}

        Observer(Observer&& other) noexcept:
            _value(std::move(other._value))
        {}

        void OnNext(TInput value) override
        {
            if(_value != value)
            {
                _value = value;
            }
        }

        void Set(TInput value)
        {
            if(_value != value)
            {
                _value = value;
            }
        }

        TInput Get() const
        {
            return TInput(_value);
        }

        operator TInput() const
        {
            return TInput(_value);
        }

        Observer& operator=(TInput value)
        {
            Set(value);
            return *this;
        }

        Observer& operator=(const Observer& other)
        {
            _value = other._value;
            return *this;
        }

        Observer& operator=(Observer&& other) noexcept
        {
            _value = std::move(other._value);
            return *this;
        }

        friend bool operator==(const Observer& left, const Observer& right)
        {
            return left._value == right._value;
        }

        const T* operator->() const { return &_value; }
    };
}