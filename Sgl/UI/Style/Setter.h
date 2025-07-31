#pragma once

namespace Sgl
{
    template<typename TTarget>
    class ISetter
    {
    public:
        virtual ~ISetter() = default;

        virtual void Apply(TTarget& target) const = 0;
    };

    template<typename TTarget, typename TProperty>
    class Setter: public ISetter<TTarget>
    {
    public: 
        using TValue = TProperty::Type;
        using TInput = TProperty::InputType;
        using TField = TProperty TTarget::*;

        TField Field;
        TValue Value;
    public:
        Setter(TField field, TInput value):
            Field(field), Value(value)
        {}

        void Apply(TTarget& target) const
        {
            (target.*Field).Set(Value);
        }
    };
}