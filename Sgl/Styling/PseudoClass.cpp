#include "PseudoClass.h"
#include "../Base/Exceptions.h"

namespace Sgl
{
    static constexpr size_t MaxPseudoClassId = (sizeof(PseudoClassId) * 8) - 1;

    PseudoClassId PseudoClassesRegistry::Register(std::string_view name)
    {        
        if(_nextId == MaxPseudoClassId)
        {
            throw Exception("Unable to register preudoclass. Limit reached - {}.", MaxPseudoClassId + 1);
        }

        auto id = _nextId++;

        if(_nameToId.contains(name))
        {
            throw Exception("Pseudoclass with name '{}' already registered.", name);
        }

        _nameToId.emplace(name, id);
        return id;
    }

    PseudoClassId PseudoClassesRegistry::GetByName(std::string_view name)
    {        
        if(auto it = _nameToId.find(name); it != _nameToId.end())
        {
            return it->second;
        }

        throw Exception("Trigger with name '{}' is not registered.", name);
    }

    PseudoClassesSet::PseudoClassesSet(PseudoClasses classes):
        _classes(classes)
    {}

    PseudoClassesSet::PseudoClassesSet(const PseudoClassesSet& other):
        _classes(other._classes)
    {}

    void PseudoClassesSet::Set(PseudoClassId pseudoClass, bool value)
    {
        if(_classes.test(pseudoClass) != value)
        {
            _classes.set(pseudoClass, value);
            Changed.Invoke(*this);
        }
    }

    void PseudoClassesSet::Reset(PseudoClassId pseudoClass)
    {
        if(_classes.test(pseudoClass))
        {
            _classes.reset(pseudoClass);
            Changed.Invoke(*this);
        }
    }

    bool PseudoClassesSet::IsEmpty() const noexcept
    {
        return _classes.none();
    }

    bool PseudoClassesSet::Has(PseudoClassId pseudoClass) const
    {
        return _classes.test(pseudoClass);
    }

    bool PseudoClassesSet::Has(PseudoClasses pseudoClasses) const
    {
        return (_classes & pseudoClasses) == pseudoClasses;
    }

    bool PseudoClassesSet::Has(const PseudoClassesSet& pseudoClasses) const
    {
        return (_classes & pseudoClasses._classes) == pseudoClasses._classes;
    }
}