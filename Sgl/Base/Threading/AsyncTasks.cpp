#include "AsyncTasks.h"

namespace Sgl
{
    TimeAwaitable AsyncTasks::Delay(TimeSpan duration)
    {
        return TimeAwaitable(duration);
    }

    TimeAwaitable AsyncTasks::Delay(TimeSpan duration, std::stop_token stopToken)
    {
        return TimeAwaitable(duration, stopToken);
    }

    TimeAwaitable AsyncTasks::Delay(size_t milliseconds)
    {
        return TimeAwaitable(TimeSpan::FromMilliseconds(milliseconds));
    }

    TimeAwaitable AsyncTasks::Delay(size_t milliseconds, std::stop_token stopToken)
    {
        return TimeAwaitable(TimeSpan::FromMilliseconds(milliseconds), stopToken);
    }
}

