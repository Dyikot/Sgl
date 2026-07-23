#pragma once

#include "../Base/Logging.h"
#include "../Base/Media/ResourceKey.h"
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

        Style& Set(std::unique_ptr<Setter> setter)
        {
            _setters.push_back(std::move(setter));
            return *this;
        }

        template<typename TOwner, typename TValue>
        Style& Set(StyleableProperty<TOwner, TValue>& property,
                   StyleableProperty<TOwner, TValue>::Value value)
        {
            _setters.emplace_back(new ValueSetter<TOwner, TValue>(property, value));
            return *this;
        }

        template<typename TOwner, typename TValue>
        Style& Set(StyleableProperty<TOwner, TValue>& property, const ResourceKey& key)
        {
            _setters.emplace_back(new ResourceSetter<TOwner, TValue>(property, key));
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
        std::vector<std::unique_ptr<Setter>> _setters;

        friend class StyleableElement;
    };    
}