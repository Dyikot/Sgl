#include "TaskOperations.h"

namespace Sgl
{
    TimeAwaitable TaskOperations::Delay(TimeSpan duration)
    {
        return TimeAwaitable(duration);
    }

    TimeAwaitable TaskOperations::Delay(size_t milliseconds)
    {
        return TimeAwaitable(TimeSpan::FromMilliseconds(milliseconds));
    }
}

