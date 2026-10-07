#include "Style.h"
#include "Styleable.h"

namespace
{
    using namespace Sgl;

    struct ByProperty
    {
        PropertyBase* property;

        bool operator()(const Ref<ISetter>& setter)
        {
            return setter->GetProperty() == *property;
        }
    };
}

namespace Sgl
{
    void Style::Apply(Styleable& element) const
    {
        for(auto& setter : setters)
        {
            setter->Apply(element, ValueSource::Style);
        }
    }

    void Style::ApplyStates(Styleable& element) const
    {
        for(auto& [state, setters] : states)
        {
            if(!element.States.Has(state))    
            {
                continue;
            }

            for(auto& setter : setters)
            {
                setter->Apply(element, ValueSource::VisualState);
            }
        }
    }

    void Style::Merge(const Style& other)
    {
        Merge(setters, other.setters);

        for(auto& [state, source] : other.states)
        {
            auto& target = states[state];
            Merge(target, source);
        }
    }

    void Style::Merge(SetterCollection& target, const SetterCollection& source)
    {
        for(auto& setter : source)
        {
            auto& property = setter->GetProperty();
            auto it = std::ranges::find_if(target, ByProperty(&property));

            if(it != target.end())
            {
                *it = setter;
            }
            else
            {
                target.Add(setter);
            }
        }
    }
}