#include "AsyncTasks.h"

namespace Sgl
{
    TimeAwaitable AsyncTasks::Delay(TimeSpan duration)
    {
        return TimeAwaitable(duration);
    }

    TimeAwaitable AsyncTasks::Delay(size_t milliseconds)
    {
        return TimeAwaitable(TimeSpan::FromMilliseconds(milliseconds));
    }
}

