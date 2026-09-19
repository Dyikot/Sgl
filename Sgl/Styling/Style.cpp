#include "Style.h"
#include "Styleable.h"

namespace Sgl
{
    static bool Contains(const std::vector<std::string>& classes, const std::string& className)
    {
        for(auto& aclass : classes)
        {
            if(aclass == className)
            {
                return true;
            }
        }

        return false;
    }

    Style::Style(Style&& other) noexcept: 
        _setters(std::move(other._setters)),
        _typeComparer(other._typeComparer),
        _name(std::exchange(other._name, nullptr)),
        _classes(std::exchange(other._classes, nullptr)),
        _pseudoClasses(other._pseudoClasses)
    {}

    Style::~Style()
    {
        delete _name;
        delete _classes;
    }

    Style& Style::Name(std::string name)
    {
        if(!_name)
        {
            _name = new std::string();
        }

        _name->swap(name);

        return *this;
    }

    Style& Style::Class(std::string className)
    {
        if(!_classes)
        {
            _classes = new std::vector<std::string>();
        }

        _classes->push_back(std::move(className));
        return *this;
    }

    Style& Style::On(PseudoClass pseudoClass)
    {
        _pseudoClasses.set(pseudoClass.GetId());
        return *this;
    }

    Style& Style::On(std::string_view pseudoClassName)
    {
        _pseudoClasses.set(PseudoClass::GetByName(pseudoClassName).GetId());
        return *this;
    }

    Style& Style::Target(TargetSelector targetSelector)
    {
        _targetSelector = std::move(targetSelector);
        return *this;
    }

    Style& Style::Set(std::unique_ptr<Setter> setter)
    {
        _setters.push_back(std::move(setter));
        return *this;
    }

    bool Style::Match(const Styleable& element) const
    {
        if(_typeComparer && !_typeComparer(element))
        {
            return false;
        }

        if(_name && element.Name != *_name)
        {
            return false;
        }

        if(_classes)
        {
            auto& targetClasses = element.GetClasses();

            for(auto& className : *_classes)
            {
                if(!Contains(targetClasses, className))
                {
                    return false;
                }
            }
        }

        return true;
    }

    bool Style::HasState() const
    {
        return _pseudoClasses.any();
    }

    bool Style::MatchState(const Styleable& element) const
    {
        return element.PseudoClasses.Has(_pseudoClasses);
    }

    void Style::Apply(Styleable& element, ValueSource source) const
    {
        auto& target = SelectTarget(element);

        for(auto& setter : _setters)
        {
            setter->Apply(target, source);
        }
    }

    void Style::Save(Styleable& element, std::vector<std::unique_ptr<ISavedValue>>& values) const
    {
        auto& target = SelectTarget(element);

        for(auto& setter : _setters)
        {
            values.emplace_back(setter->Save(target));
        }
    }

    Styleable& Style::SelectTarget(Styleable& element) const
    {
        return _targetSelector ? _targetSelector(element) : element;
    }
}