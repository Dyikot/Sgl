#pragma once

#include <concepts>
#include "Delegate.h"

namespace Sgl
{
    template<typename T, typename TInput = T> 
        requires std::constructible_from<T, TInput> && 
                 std::convertible_to<T, TInput> && 
                 std::copyable<T>
    class ObservableProperty
    {
    public:
        using Type = T;
        using InputType = TInput;
    protected:
        T _value;
    private:
        Action<TInput> _observer;
        bool _isChanging;
    public:
        ObservableProperty() requires std::default_initializable<T>:
            _value(),
            _isChanging(false)
        {}

        ObservableProperty(TInput value):
            _value(value),
            _isChanging(false)
        {}

        ObservableProperty(const ObservableProperty& other):
            _value(other._value),
            _observer(other._observer),
            _isChanging(other._isChanging)
        {}

        ObservableProperty(ObservableProperty&& other) noexcept:
            _value(std::move(other._value)),
            _observer(std::move(other._observer)),
            _isChanging(other._isChanging)
        {}

        virtual ~ObservableProperty() = default;

        void Set(TInput value)
        {
            if(!_isChanging)
            {
                _isChanging = true;
                _value = value;
                OnChanged();
                _isChanging = false;
            }
        }

        TInput Get() const 
        {
            return TInput(_value); 
        }

        void SetObserver(T& observer)
        {
            _observer = [&observer](TInput value) { observer = value; };
        }

        void SetObserver(Action<TInput> observer)
        {
            _observer = std::move(observer);
        }

        void SetObserver(ObservableProperty& observer, bool twoWay = false)
        {
            _observer = [&observer](TInput value) { observer.Set(value); };

            if(twoWay)
            {
                observer._observer = [this](TInput value) { Set(value); };
            }
        }

        template<typename TObserver>
        void SetObserver(TObserver& observer, void(TObserver::*setter)(TInput))
        {
            _observer = [&observer, setter](TInput value) { (observer.*setter)(value); };
        }

        void RemoveObserver()
        {
            _observer.Reset();
        }

        operator TInput() const 
        { 
            return TInput(_value); 
        }

        ObservableProperty& operator=(TInput value)
        {
            Set(value);
            return *this;
        }

        ObservableProperty& operator=(const ObservableProperty& other)
        {
            _value = other._value;
            _observer = other._observer;
            return *this;
        }

        ObservableProperty& operator=(ObservableProperty&& other) noexcept
        {
            _value = std::move(other._value);
            _observer = std::move(other._observer);
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