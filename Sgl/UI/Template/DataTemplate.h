#pragma once

#include "../../Base/Any.h"
#include "../../Base/SmartPointers.h"
#include "../../Base/Delegate.h"

namespace Sgl
{
    class UIElement;

    using DataTemplate = Delegate<Shared<UIElement>(const Any&)>;
}