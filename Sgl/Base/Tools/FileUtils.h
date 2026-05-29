#pragma once

#include <filesystem>
#include "../Threading/Task.h"

namespace Sgl
{
	class File
	{
	public:
		static void WriteText(const std::filesystem::path& path, const std::string& text);
		static Task<void> WriteTextAsync(const std::filesystem::path& path, const std::string& text);
		static std::string ReadText(const std::filesystem::path& path);
		static Task<std::string> ReadTextAsync(const std::filesystem::path& path);
	};
}