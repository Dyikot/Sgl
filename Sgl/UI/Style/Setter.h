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

    template<typename TTarget, typename TField, typename TValue>
    class Setter: public ISetter<TTarget>
    {
    public:
        TField Field;
        TValue Value;
    public:
        Setter(TField field, TValue value):
            Field(field), Value(value)
        {}

        void Apply(TTarget& target) const
        {
            (target.*Field).OnNext(Value);
        }
    };
}