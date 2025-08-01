#pragma once

#include <concepts>
#include "../../Base/Observable/IObserver.h"

namespace Sgl
{
    template<typename T, typename TInput = T> 
        requires std::constructible_from<T, TInput> && std::copyable<T>
    class StyleableProperty: public IObserver<TInput>
    {
    public:
        using Type = T;
        using InputType = TInput;
    protected:
        T _value;
    private:
        IObserver<TInput>* _observer;
    public:
        StyleableProperty() requires std::default_initializable<T>:
            _value(),
            _observer(nullptr)
        {}

        StyleableProperty(TInput value):
            _value(value),
            _observer(nullptr)
        {}

        StyleableProperty(const StyleableProperty& other):
            _value(other._value),
            _observer(nullptr)
        {}

        StyleableProperty(StyleableProperty&& other) noexcept:
            _value(std::move(other._value)),
            _observer(std::exchange(other._observer, nullptr))
        {}

        virtual ~StyleableProperty() = default;

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

        void SetObserver(IObserver<TInput>& observer)
        {
            _observer = &observer;
        }

        void Bind(StyleableProperty& observer)
        {
            SetObserver(observer);
            observer.SetObserver(*this);
        }

        void RemoveObserver(IObserver<TInput>& observer)
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

        StyleableProperty& operator=(TInput value)
        {
            Set(value);
            return *this;
        }

        StyleableProperty& operator=(const StyleableProperty& other)
        {
            if(this != &other)
            {
                _value = other._value;
            }

            return *this;
        }

        StyleableProperty& operator=(StyleableProperty&& other) noexcept
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