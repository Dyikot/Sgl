#include "Selector.h"
#include "StyleableElement.h"

#include <algorithm>

namespace Sgl
{
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

        auto& classes = target.GetClasses();

        if(_flags & ClassFlag)
        {
            result &= (std::ranges::find(classes, _className) != classes.end());

            if(!result)
            {
                return false;
            }
        }

        return result;
    }
}

