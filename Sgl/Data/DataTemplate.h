#pragma once

#include "../Base/Any.h"
#include "../Base/Ref.h"
#include "../Base/Delegate.h"

namespace Sgl
{
    class UIElement;

    using DataTemplate = Delegate<Ref<UIElement>(const Any&)>;
}