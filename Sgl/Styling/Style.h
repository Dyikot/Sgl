#pragma once

#include <memory>
#include <vector>

#include "../Base/Delegate.h"
#include "../Base/Media/ThemeMode.h"
#include "../Data/StyleableProperty.h"
#include "Selector.h"
#include "Setter.h"

namespace Sgl
{
    class Style
    {
    public:
        Style() = default;
        Style(const Style&) = delete;
        Style(Style&&) noexcept = default;
        Style(Sgl::Selector selector):
            Selector(std::move(selector))
        {}

        Selector Selector;

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

        template<typename TOwner, typename TValue, typename TResources, typename TResource>
        Style& Set(StyleableProperty<TOwner, TValue>& property, 
                   TResources& resources,
                   TResource TResources::* resource)
        {
            _setters.emplace_back(new ResourceSetter(property, resources, resource));
            return *this;
        }

        template<typename TOwner, typename TValue, typename TResource>
        Style& Set(StyleableProperty<TOwner, TValue>& property,
                   TResource ThemeResources::* resource)
        {
            return Set(property, GetThemeResources(), resource);
        }

        void Apply(StyleableElement& target) const
        {
            for(auto& setter : _setters)
            {
                setter->Apply(target);
            }
        }
    private:
        std::vector<std::unique_ptr<ISetter>> _setters;
    };    
}