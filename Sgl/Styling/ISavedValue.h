#pragma once

#include <concepts>
#include "../Base/Ref.h"
#include "../Data/StyleableProperty.h"

namespace Sgl
{
    class Bindable;

    class ISavedValue
    {
    public:
        virtual ~ISavedValue() = default;

        virtual void Restore() = 0;
    };

    template<typename TOwner, typename TValue> requires std::derived_from<TOwner, Bindable>
    class SavedPropertyValue final : public ISavedValue
    {
    public:
        using Value = std::decay_t<TValue>;
    public:
        SavedPropertyValue(StyleableProperty<TOwner, TValue>& property, TOwner& owner):
            _property(property),
            _owner(&owner),
            _value(property.InvokeGetter(owner))
        {}

        void Restore() override
        {
            _property.InvokeSetter(_owner.GetValue(), _value, ValueSource::PseudoClass);
        }

    private:
        StyleableProperty<TOwner, TValue>& _property;
        Ref<TOwner> _owner;
        Value _value;
    };
}