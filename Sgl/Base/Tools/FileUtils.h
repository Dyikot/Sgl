#pragma once

#include <filesystem>
#include "../Threading/Task.h"

namespace Sgl
{
	//! @brief Provides static methods for reading and writing text files
	class File
	{
	public:
		//! @brief Writes the specified text to a file synchronously
		//! @param path The path to the file to write to
		//! @param text The text to write to the file
		static void WriteText(const std::filesystem::path& path, const std::string& text);

		//! @brief Asynchronously writes the specified text to a file
		//! @param path The path to the file to write to
		//! @param text The text to write to the file
		//! @return A task representing the asynchronous write operation
		static Task<void> WriteTextAsync(const std::filesystem::path& path, const std::string& text);

		//! @brief Reads all text from a file synchronously
		//! @param path The path to the file to read
		//! @return The text content of the file
		static std::string ReadText(const std::filesystem::path& path);

		//! @brief Asynchronously reads all text from a file
		//! @param path The path to the file to read
		//! @return A task representing the asynchronous read operation, which yields the text content of the file
		static Task<std::string> ReadTextAsync(const std::filesystem::path& path);
	};
}