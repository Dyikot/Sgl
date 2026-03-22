#pragma once

#include <memory>
#include <vector>

#include "../Base/Logging.h"
#include "../Data/StyleableProperty.h"
#include "Selector.h"
#include "Setter.h"
#include "Projection.h"

namespace Sgl
{
    class Style
    {
    public:
        Style(Sgl::Selector selector): 
            Selector(std::move(selector)),
            Projection(nullptr)
        {}

        Style(Sgl::Selector selector, TargetProjection projection):
            Selector(std::move(selector)),
            Projection(std::move(projection))
        {}

        Style(const Style&) = delete;
        Style(Style&&) noexcept = default;

        const Selector Selector;
        const TargetProjection Projection;

        template<typename TOwner, typename TValue>
        Style& Set(StyleableProperty<TOwner, TValue>& property,
                   StyleableProperty<TOwner, TValue>::Value value)
        {
            _setters.emplace_back(new Setter<TOwner, TValue>(property, value));
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
    private:
        void Apply(StyleableElement& target, ValueSource source) const
        {
            auto& targetElement = Projection ? Projection(target) : target;

            for(auto& setter : _setters)
            {
                setter->Apply(targetElement, source);
            }
        }
    private:
        std::vector<std::unique_ptr<SetterBase>> _setters;

        friend class StyleableElement;
    };    
}