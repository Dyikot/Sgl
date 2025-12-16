#include "FileAwaitables.h"

#include <sstream>
#include <fstream>
#include "../Exceptions.h"

namespace Sgl
{
	ReadFileAwaitable::ReadFileAwaitable(std::string path, bool saveContext):
		TaskAwaitable<std::string>(
            [path = std::move(path)]
            {
                if(auto stream = std::ifstream(path))
                {
                    std::ostringstream ss;
                    ss << stream.rdbuf();
                    return std::move(ss).str();
                }

                throw Exception("Unable ro read text. Unable open file '{}'.", path);

            }, saveContext
        )
	{}

    WriteFileAwaitable::WriteFileAwaitable(std::string path, std::string text):
        TaskAwaitable<void>(
            [path = std::move(path), text = std::move(text)]
            {
                if(auto stream = std::ofstream(path))
                {
                    stream << text;
                }

            }, false
        )
    {}
}

