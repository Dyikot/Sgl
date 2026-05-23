#include "PseudoClass.h"
#include "../Base/Exceptions.h"

namespace Sgl
{
    static constexpr size_t MaxPseudoClassId = (sizeof(size_t) * 8) - 1;

    PseudoClass::PseudoClass(std::string_view name)
    {
        auto nextId = _registry.size();

        if(nextId == MaxPseudoClassId)
        {
            throw Exception("Unable to register preudoclass. Limit reached - {}.", MaxPseudoClassId + 1);
        }

        if(_registry.contains(name))
        {
            throw Exception("Pseudoclass with name '{}' already registered.", name);
        }

        _registry.emplace(name, nextId);
        _id = nextId;
    }

    PseudoClass::PseudoClass(size_t id):
        _id(id)
    {}

    PseudoClass PseudoClass::GetByName(std::string_view name)
    {
        if(auto it = _registry.find(name); it != _registry.end())
        {
            return PseudoClass(it->second);
        }

        throw Exception("Pseudoclass with name '{}' is not registered.", name);
    }

    PseudoClassesSet::PseudoClassesSet(PseudoClasses classes):
        _classes(classes)
    {}

    PseudoClassesSet::PseudoClassesSet(const PseudoClassesSet& other):
        _classes(other._classes)
    {}

    void PseudoClassesSet::Set(PseudoClass pseudoClass, bool value)
    {
        if(_classes.test(pseudoClass.GetId()) != value)
        {
            _classes.set(pseudoClass.GetId(), value);
            Changed.Invoke(*this);
        }
    }

    void PseudoClassesSet::Reset(PseudoClass pseudoClass)
    {
        if(_classes.test(pseudoClass.GetId()))
        {
            _classes.reset(pseudoClass.GetId());
            Changed.Invoke(*this);
        }
    }

    bool PseudoClassesSet::IsEmpty() const noexcept
    {
        return _classes.none();
    }

    bool PseudoClassesSet::Has(PseudoClass pseudoClass) const
    {
        return _classes.test(pseudoClass.GetId());
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