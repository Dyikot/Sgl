#pragma once

#include "../Base/Logging.h"
#include "../Base/Media/ResourceKey.h"
#include "../Data/StyleableProperty.h"
#include "Selector.h"
#include "Setter.h"
#include "TargetSelector.h"

namespace Sgl
{
    class Style
    {
    public:
        Style(Sgl::Selector selector): 
            _selector(std::move(selector))
        {}

        Style(const Style&) = delete;

        Style(Style&& other) noexcept:
            _setters(std::move(other._setters)),
            _targetSelector(std::move(other._targetSelector)),
            _selector(std::move(other._selector))
        {}
                
        const Selector& GetSelector() const
        {
            return _selector;
        }

        Style& Target(TargetSelector targetSelector)
        {
            _targetSelector = std::move(targetSelector);
            return *this;
        }

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
        Styleable& SelectTarget(Styleable& element) const
        {
            return _targetSelector ? _targetSelector(element) : element;
        }

        void Apply(Styleable& element, ValueSource source) const
        {
            auto& target = SelectTarget(element);

            for(auto& setter : _setters)
            {
                setter->Apply(target, source);
            }
        }

    private:
        std::vector<std::unique_ptr<Setter>> _setters;
        TargetSelector _targetSelector;
        Selector _selector;

        friend class Styleable;
    };    
}