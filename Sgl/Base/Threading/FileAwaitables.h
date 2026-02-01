#pragma once

#include <string>
#include "TaskAwaitable.h"

namespace Sgl
{
	/// <summary>
	/// An awaitable object that asynchronously reads the contents of a file as a std::string.
	/// Inherits from TaskAwaitable<std::string> and uses the ThreadPool to perform the I/O operation off the main thread.
	/// When awaited in a coroutine, it suspends execution until the file read completes and returns the file content.
	/// </summary>
	class ReadFileAwaitable : public TaskAwaitable<std::string>
	{
	public:
		explicit ReadFileAwaitable(std::string path, bool saveContext = false);
	};

	/// <summary>
	/// An awaitable object that asynchronously writes a string to a file.
	/// Inherits from TaskAwaitable<void> and uses the ThreadPool to perform the write operation off the main thread.
	/// When awaited in a coroutine, it suspends execution until the write operation completes.
	/// </summary>
	class WriteFileAwaitable : public TaskAwaitable<void>
	{
	public:
		WriteFileAwaitable(std::string path, std::string text);
	};
}