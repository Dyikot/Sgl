#include "VisualState.h"
#include "../Base/Exceptions.h"

namespace Sgl
{
    VisualState::VisualState(size_t id):
        _id(id)
    {}

    VisualState VisualState::Register(std::string name)
    {
        auto nextId = _registry.size();

        if(nextId == MaxId)
        {
            throw Exception("Unable to register visual state. Limit reached - {}.", MaxId + 1);
        }

        if(_registry.contains(name))
        {
            throw Exception("VisualState with name '{}' already registered.", name);
        }

        _registry.emplace(std::move(name), nextId);
        return VisualState(nextId);
    }   

    void VisualStateSet::Set(VisualState state, bool value)
    {
        auto id = state.GetId();
        if(_states.test(id) != value)
        {
            _states.set(id, value);
            Changed.Invoke(*this);
        }
    }

    void VisualStateSet::Reset(VisualState state)
    {
        auto id = state.GetId();
        if(_states.test(id))
        {
            _states.reset(id);
            Changed.Invoke(*this);
        }
    }

    bool VisualStateSet::IsEmpty() const noexcept
    {
        return _states.none();
    }

    bool VisualStateSet::Has(VisualState state) const
    {
        return _states.test(state.GetId());
    }

    bool VisualStateSet::Has(const VisualStateSet& states) const
    {
        return (_states & states._states) == states._states;
    }
}