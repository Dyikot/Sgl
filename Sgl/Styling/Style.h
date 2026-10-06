#pragma once

#include "../Base/Collections/PackedMap.h"
#include "Selector.h"
#include "VisualState.h"
#include "SetterCollection.h"

namespace Sgl
{
    struct Style
    {   
    public:
        Selector selector;
        SetterCollection setters;
        PackedMap<VisualState, SetterCollection> states;

        void Apply(Styleable& element) const;
        void ApplyStates(Styleable& element) const;
        void Merge(const Style& other);
    private:
        static void Merge(SetterCollection target, SetterCollection source);
    };
}