#include "FileAwaitables.h"

#include <fstream>
#include "../Exceptions.h"

namespace Sgl
{
	ReadFileAwaitable::ReadFileAwaitable(const std::filesystem::path& path, bool saveContext):
		TaskAwaitable<std::string>(
            [path]
            {
                if(auto stream = std::ifstream(path, std::ios::binary))
                {
                    const auto size = std::filesystem::file_size(path);
                    std::string text(size, '\0');

                    stream.read(text.data(), size);
                    return text;
                }

                throw Exception("Unable ro read text. Unable open file '{}'.", path.string());

            }, saveContext
        )
	{}

    WriteFileAwaitable::WriteFileAwaitable(const std::filesystem::path& path, std::string text):
        TaskAwaitable<void>(
            [path, text = std::move(text)]
            {
                if(auto stream = std::ofstream(path))
                {
                    stream << text;
                }

            }, false
        )
    {}
}

