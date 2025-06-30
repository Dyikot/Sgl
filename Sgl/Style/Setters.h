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

        template<typename TField, typename TValue>
        void Add(TField field, TValue value)
        {
            _setters.push_back([field, value](T& target)
            {
                target.*field = value;
            });
        }
    };
}