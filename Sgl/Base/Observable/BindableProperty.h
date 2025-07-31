#pragma once

#include <concepts>
#include "IObservable.h"

namespace Sgl
{
    template<typename T, typename TInput = T> 
        requires std::constructible_from<T, TInput> && std::copyable<T>
    class BindableProperty: public IObserver<TInput>, public IObservable<TInput>
    {
    public:
        using Type = T;
        using InputType = TInput;
    protected:
        T _value;
    private:
        IObserver<TInput>* _observer;
    public:
        BindableProperty() requires std::default_initializable<T>:
            _value(),
            _observer(nullptr)
        {}

        BindableProperty(TInput value):
            _value(value),
            _observer(nullptr)
        {}

        BindableProperty(const BindableProperty& other):
            _value(other._value),
            _observer(nullptr)
        {}

        BindableProperty(BindableProperty&& other) noexcept:
            _value(std::move(other._value)),
            _observer(std::exchange(other._observer, nullptr))
        {}

        virtual ~BindableProperty() = default;

        void OnNext(TInput value) override
        {
            if(_value != value)
            {
                _value = value;
                OnChanged();
            }
        }

        void Set(TInput value)
        {
            if(_value != value)
            {
                _value = value;
                OnChanged();
            }
        }

        TInput Get() const 
        {
            return TInput(_value); 
        }

        void Subscribe(IObserver<TInput>& observer) override
        {
            _observer = &observer;
        }

        void Bind(BindableProperty& observer)
        {
            Subscribe(observer);
            observer.Subscribe(*this);
        }

        void Unsubscribe(IObserver<TInput>& observer) override
        {
            if(_observer == &observer)
            {
                _observer = nullptr;
            }
        }

        bool HasObserver() const 
        {
            return _observer != nullptr;
        }

        operator TInput() const 
        { 
            return TInput(_value); 
        }

        BindableProperty& operator=(TInput value)
        {
            OnNext(value);
            return *this;
        }

        BindableProperty& operator=(const BindableProperty& other)
        {
            if(this != &other)
            {
                _value = other._value;
            }

            return *this;
        }

        BindableProperty& operator=(BindableProperty&& other) noexcept
        {
            _value = std::move(other._value);
            _observer = std::exchange(other._observer, nullptr);

            return *this;
        }

        const T* operator->() const { return &_value; }
    protected:
        virtual void OnChanged()
        {
            if(_observer)
            {
                _observer->OnNext(_value);
            }
        }
    };
}