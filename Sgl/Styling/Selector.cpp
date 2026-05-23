#include "Selector.h"
#include "StyleableElement.h"

namespace Sgl
{
    static bool Contains(const std::vector<std::string> &classes, const std::string& className)
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

    Selector& Selector::Name(std::string name)
    {
        _name = std::move(name);
        _flags |= NameFlag;
        return *this;
    }

    Selector& Selector::Class(std::string className)
    {
        _classes.push_back(std::move(className));
        _flags |= ClassFlag;
        return *this;
    }

    Selector& Selector::On(PseudoClass pseudoClass)
    {
        _pseudoClasses.set(pseudoClass.GetId());
        return *this;
    }

    Selector& Selector::On(std::string_view pseudoClassName)
    {
        _pseudoClasses.set(PseudoClass::GetByName(pseudoClassName).GetId());
        return *this;
    }

    Selector& Selector::Where(Predicate<const StyleableElement&> predicate)
    {
        _predicate = std::move(predicate);
        _flags |= PredicateFlag;
        return *this;
    }

    bool Selector::Match(const StyleableElement& target) const
    {
        bool result = true;

        if(_flags & TypeFlag)
        {
            result &= _typeComparer(target);

            if(!result)
            {
                return false;
            }
        }

        if(_flags & NameFlag)
        {
            result &= target.Name == _name;

            if(!result)
            {
                return false;
            }
        }

        if(_flags & ClassFlag)
        {
            auto& classes = target.GetClasses();

            for(auto& className : _classes)
            {
                result &= Contains(classes, className);

                if(!result)
                {
                    return false;
                }
            }
        }

        if(_flags & PredicateFlag)
        {
            result &= _predicate(target);

            if(!result)
            {
                return false;
            }
        }

        return result;
    }

    bool Selector::MatchState(const StyleableElement& target) const
    {
        return target.PseudoClasses.Has(_pseudoClasses);
    }

    bool Selector::HasState() const noexcept
    {
        return _pseudoClasses.any();
    }
}

