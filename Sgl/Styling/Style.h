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
            _projection(nullptr)
        {}

        Style(Sgl::Selector selector, TargetProjection projection):
            Selector(std::move(selector)),
            _projection(std::move(projection))
        {}

        Style(const Style&) = delete;
        Style(Style&&) noexcept = default;

        const Selector Selector;        

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

        StyleableElement& SelectTarget(StyleableElement& element) const
        {
            return _projection ? _projection(element) : element;
        }
    private:
        void Apply(StyleableElement& element, ValueSource source) const
        {
            auto& target = SelectTarget(element);

            for(auto& setter : _setters)
            {
                setter->Apply(target, source);
            }
        }
    private:
        std::vector<std::unique_ptr<Setter>> _setters;
        TargetProjection _projection;

        friend class StyleableElement;
    };    
}