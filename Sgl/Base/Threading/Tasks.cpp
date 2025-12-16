#include "Tasks.h"

#include <sstream>
#include <fstream>

namespace Sgl
{
    DelayAwaitable Tasks::Delay(TimeSpan duration)
    {
        return DelayAwaitable(duration);
    }

    DelayAwaitable Tasks::Delay(TimeSpan duration, std::stop_token stopToken)
    {
        return DelayAwaitable(duration, stopToken);
    }

    DelayAwaitable Tasks::Delay(size_t milliseconds)
    {
        return DelayAwaitable(TimeSpan::FromMilliseconds(milliseconds));
    }

    DelayAwaitable Tasks::Delay(size_t milliseconds, std::stop_token stopToken)
    {
        return DelayAwaitable(TimeSpan::FromMilliseconds(milliseconds), stopToken);
    }

    TaskAwaitable<std::string> Tasks::ReadTextAsync(std::string path, bool saveContext)
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

    TaskAwaitable<void> Tasks::WriteTextAsync(std::string path, std::string text)
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

