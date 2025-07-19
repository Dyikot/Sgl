#pragma once

#include <memory>
#include <vector>
#include "IStyle.h"
#include "../../Base/Delegate.h"

namespace Sgl
{
    using std::shared_ptr;    

    template<typename T>
    class Style: public IStyle
    {
    private:
        std::vector<Action<T&>> _setters;
    public:
        Style() = default;

        auto begin() { return _setters.begin(); }
        auto end() { return _setters.end(); }

        auto begin() const { return _setters.begin(); }
        auto end() const { return _setters.end(); }

        template<typename TProperty>
        Style<T>& AddSetter(TProperty T::* field, TProperty::InputType value)
        {
            _setters.push_back([field, value = TProperty::Type(value)](T& target)
            {
                (target.*field).Set(value);
            });

            return *this;
        }

        void Apply(StyleableElement& target) override
        {
            T& targetElement = static_cast<T&>(target);
            for(const auto& setter : _setters)
            {
                setter(targetElement);
            }
        }
    };    
}