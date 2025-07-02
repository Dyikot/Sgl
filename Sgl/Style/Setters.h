#pragma once

#include <vector>
#include "../Data/Delegate.h"

namespace Sgl
{
    template<typename T>
    class SettersCollection
    {
    public:
        using Setter = Action<T&>;
    private:
        std::vector<Setter> _setters;
    public:
        auto begin() const { return _setters.begin(); }
        auto end() const { return _setters.end(); }

        template<typename TProperty>
        void Add(TProperty T::* field, TProperty::InputType value)
        {
            _setters.push_back([field, value = TProperty::Type(value)](T& target)
            {
                (target.*field).Set(value);
            });
        }
    };
}