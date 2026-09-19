#pragma once

#include "../Base/Delegate.h"
#include "../Base/Media/ResourceKey.h"
#include "Setter.h"
#include "PseudoClass.h"

namespace Sgl
{
    class Style
    {
    public:
        using PseudoClasses = std::bitset<64>;
        using TypeComparer = bool(*)(const Styleable&);
        using TargetSelector = Func<Styleable&, Styleable&>;
    public:
        Style() = default;
        Style(const Style&) = delete;
        Style(Style&& other) noexcept;
        ~Style();
                
        template<typename T>
        Style& OfType()
        {
            _typeComparer = CompareTypeById<T>;
            return *this;
        }

        template<typename T>
        Style& Is()
        {
            _typeComparer = CompareType<T>;
            return *this;
        }

        Style& Name(std::string name);
        Style& Class(std::string className);
        Style& On(PseudoClass pseudoClass);
        Style& On(std::string_view pseudoClassName);       
        Style& Target(TargetSelector targetSelector);

        Style& Set(std::unique_ptr<Setter> setter);

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

        bool HasState() const;
        bool Match(const Styleable& element) const;
        bool MatchState(const Styleable& element) const;
        void Apply(Styleable& element, ValueSource source) const;
        void Save(Styleable& element, std::vector<std::unique_ptr<ISavedValue>>& values) const;
    private:
        Styleable& SelectTarget(Styleable& element) const;

        template<typename T>
        static bool CompareType(const Styleable& element)
        {
            return dynamic_cast<const T*>(&element);
        }

        template<typename T>
        static bool CompareTypeById(const Styleable& element)
        {
            return typeid(T) == typeid(element);
        }

    private:
        TypeComparer _typeComparer {};
        std::string* _name {};
        std::vector<std::string>* _classes {};
        PseudoClasses _pseudoClasses;
        TargetSelector _targetSelector;
        std::vector<std::unique_ptr<Setter>> _setters;
    };
}