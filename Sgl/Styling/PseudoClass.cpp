#include "PseudoClass.h"
#include "../Base/Exceptions.h"

namespace Sgl
{
    static constexpr size_t MaxStates = (sizeof(PseudoClassId) * 8) - 1;

    PseudoClassId PseudoClassesRegistry::Register(std::string_view name)
    {        
        if(_nextId == MaxStates)
        {
            throw Exception("Unable to register preudoclass. Limit reached - {}.", MaxStates + 1);
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
}