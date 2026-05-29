#include "FileUtils.h"
#include "../Exceptions.h"
#include "../Threading/TaskAwaitable.h"
#include <fstream>

namespace Sgl
{
	void File::WriteText(const std::filesystem::path& path, const std::string& text)
	{
		if(auto stream = std::ofstream(path))
		{
			stream << text;
		}
	}

	Task<void> File::WriteTextAsync(const std::filesystem::path& path, const std::string& text)
	{
		co_return co_await TaskAwaitable([path, text] { return WriteText(path, text); }, false);
	}

	std::string File::ReadText(const std::filesystem::path& path)
	{
		if(auto stream = std::ifstream(path, std::ios::binary))
		{
			const auto size = std::filesystem::file_size(path);
			std::string text(size, '\0');

			stream.read(text.data(), size);
			return text;
		}

		throw Exception("Unable ro read text. Unable open file '{}'.", path.string());
	}

	Task<std::string> File::ReadTextAsync(const std::filesystem::path& path)
	{
		co_return co_await TaskAwaitable([path] { return File::ReadText(path); }, true);
	}
}

