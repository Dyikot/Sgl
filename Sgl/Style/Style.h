#pragma once

#include <string>
#include <memory>
#include "../Data/Object.h"

using std::shared_ptr;

namespace Sgl
{
    enum class Trigger
    {
        OnInitialize, OnHover, OnPressed
    };

    class IStyle
    {
    public:
        virtual ~IStyle() = default;

        virtual void ApplyTo(object target) = 0;
    };

    template<typename T>
    class Style: public IStyle
    {
    public:
        using StyleSetter = void(*)(T&);

        Trigger Trigger;
        StyleSetter Setter;
    public:
        Style():
            Trigger(Sgl::Trigger::OnInitialize),
            Setter(nullptr)
        {}

        static shared_ptr<Style<T>> New() { return std::make_shared<Style<T>>(); }

        void ApplyTo(object target) override
        {
            Setter(target.As<T>());
        }
    };
}