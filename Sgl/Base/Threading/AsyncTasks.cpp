#include "AsyncTasks.h"

#include <fstream>
#include <stdexcept>

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

    TaskAwaitable<std::string> AsyncTasks::ReadText(std::string path, bool saveContext)
    {
        return TaskAwaitable<std::string>([path = std::move(path)]
        {
            if(auto stream = std::ifstream(path))
            {
                std::ostringstream ss;
                ss << stream.rdbuf();
                return std::move(ss).str();
            }

            auto error = std::format("Unable ro read text. Unable open file '{}'.", path);
            throw std::runtime_error(error);

        }, saveContext);
    }

    TaskAwaitable<void> AsyncTasks::WriteText(std::string path, std::string text)
    {
        return TaskAwaitable<void>([path = std::move(path), text = std::move(text)]
        {
            if(auto stream = std::ofstream(path))
            {
                stream << text;
            }

        }, false);
    }
}

