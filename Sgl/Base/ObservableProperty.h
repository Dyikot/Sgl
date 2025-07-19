#pragma once

#include <concepts>
#include "Delegate.h"

namespace Sgl
{
    template<typename T, typename TInput = T> 
        requires std::constructible_from<T, TInput> && std::copyable<T>
    class ObservableProperty
    {
    public:
        using Type = T;
        using InputType = TInput;
    protected:
        T _value;
    private:
        Action<TInput> _observer;
    public:
        ObservableProperty() requires std::default_initializable<T>:
            _value()
        {}

        ObservableProperty(TInput value):
            _value(value)
        {}

        ObservableProperty(const ObservableProperty& other):
            _value(other._value)
        {}

        ObservableProperty(ObservableProperty&& other) noexcept:
            _value(std::move(other._value))
        {}

        virtual ~ObservableProperty() = default;

        void Set(TInput value)
        {
            _value = value;
            OnChanged();
        }

        TInput Get() const 
        {
            return _value; 
        }

        void Subscribe(T& observer)
        {
            _observer = [&observer](TInput value) { observer = value; };
        }

        void Subscribe(Action<TInput> observer)
        {
            _observer = std::move(observer);
        }

        void Subscribe(ObservableProperty& observer)
        {
            _observer = [&observer](TInput value) { observer.Set(value); };
        }

        template<typename TObserver>
        void Subscribe(TObserver& observer, void(TObserver::*setter)(TInput))
        {
            _observer = [&observer, setter](TInput value) { (observer.*setter)(value); };
        }

        void ClearSubcription()
        {
            _observer.Reset();
        }

        operator TInput() const 
        { 
            return _value; 
        }

        ObservableProperty& operator=(TInput value)
        {
            Set(value);
            return *this;
        }

        ObservableProperty& operator=(const ObservableProperty& other)
        {
            _value = other._value;
            return *this;
        }

        ObservableProperty& operator=(ObservableProperty&& other) noexcept
        {
            _value = std::move(other._value);
            return *this;
        }

        const T* operator->() const { return &_value; }
    protected:
        virtual void OnChanged()
        {
            if(_observer.HasTarget())
            {
                _observer(_value);
            }
        }
    };
}