#pragma once

#include <memory>
#include <vector>

#include "../Data/StyleableProperty.h"
#include "../Base/Delegate.h"
#include "Selector.h"
#include "Setter.h"

namespace Sgl
{
    class Style
    {
    public:
        Selector Selector;
    private:        
        std::vector<std::unique_ptr<ISetter>> _setters;
    public:
        Style() = default;
        Style(Sgl::Selector selector):
            Selector(std::move(selector))
        {}

        Style& Set(std::unique_ptr<ISetter> setter)
        {
            _setters.push_back(std::move(setter));
            return *this;
        }

        template<typename TOwner, typename TValue>
        Style& Set(StyleableProperty<TOwner, TValue>& property,
                   StyleableProperty<TOwner, TValue>::Value value)
        {
            _setters.emplace_back(new Setter(property, value));
            return *this;
        }

        template<typename TOwner, typename TValue, typename TResourceValue = StyleableProperty<TOwner, TValue>::Value>
        Style& Set(StyleableProperty<TOwner, TValue>& property, 
                   const ThemeResource<TResourceValue>& resource)
        {
            _setters.emplace_back(new ThemeResourceSetter(property, resource));
            return *this;
        }

        void Apply(StyleableElement& target) const
        {
            for(auto& setter : _setters)
            {
                setter->Apply(target);
            }
        }
    };    
}