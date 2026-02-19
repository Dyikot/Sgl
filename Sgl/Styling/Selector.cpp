#include "Selector.h"
#include "StyleableElement.h"

#include <algorithm>

namespace Sgl
{
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

    Selector& Selector::On(PseudoClassId pseudoClass)
    {
        _pseoduClasses.set(pseudoClass);
        _flags |= PseudoClassFlag;
        return *this;
    }

    Selector& Selector::On(std::string_view pseudoClassName)
    {
        auto trigger = PseudoClassesRegistry::GetByName(pseudoClassName);
        return *this;
    }

    Selector& Selector::Where(Func<bool, StyleableElement&> predicate)
    {
        _predicate = std::move(predicate);
        _flags |= PredicateFlag;
        return *this;
    }

    bool Selector::Match(StyleableElement& target) const
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
                result &= (std::ranges::find(classes, className) != classes.end());

                if(!result)
                {
                    return false;
                }
            }
        }

        if(_flags & PseudoClassFlag)
        {
            result &= (target.PseudoClasses & _pseoduClasses).any();

            if(!result)
            {
                return false;
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
}

