#pragma once

#include <concepts>
#include "IObserver.h"

namespace Sgl
{
    template<typename T, typename TInput = T> 
        requires std::constructible_from<T, TInput> && std::copyable<T>
    class ObservableProperty: public IObserver<TInput>
    {
    public:
        using Type = T;
        using InputType = TInput;
    protected:
        T _value;
    private:
        IObserver<TInput>* _observer;
    public:
        ObservableProperty() requires std::default_initializable<T>:
            _value(),
            _observer(nullptr)
        {}

        ObservableProperty(TInput value):
            _value(value),
            _observer(nullptr)
        {}

        ObservableProperty(const ObservableProperty& other):
            _value(other._value),
            _observer(nullptr)
        {}

        ObservableProperty(ObservableProperty&& other) noexcept:
            _value(std::move(other._value)),
            _observer(std::exchange(other._observer, nullptr))
        {}

        virtual ~ObservableProperty() = default;

        void OnNext(TInput value) override
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

        void SetObserver(IObserver<TInput>& observer)
        {
            _observer = &observer;
        }

        void Bind(ObservableProperty& observer)
        {
            SetObserver(observer);
            observer.SetObserver(*this);
        }

        void RemoveObserver()
        {
            _observer = nullptr;
        }

        bool HasObserver() const 
        {
            return _observer != nullptr;
        }

        operator TInput() const 
        { 
            return TInput(_value); 
        }

        ObservableProperty& operator=(TInput value)
        {
            OnNext(value);
            return *this;
        }

        ObservableProperty& operator=(const ObservableProperty& other)
        {
            if(this != &other)
            {
                _value = other._value;
            }

            return *this;
        }

        ObservableProperty& operator=(ObservableProperty&& other) noexcept
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