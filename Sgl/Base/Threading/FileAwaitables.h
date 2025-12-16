#pragma once

#include <string>
#include "TaskAwaitable.h"

namespace Sgl
{
	class ReadFileAwaitable : public TaskAwaitable<std::string>
	{
	public:
		explicit ReadFileAwaitable(std::string path, bool saveContext = false);
	};

	class WriteFileAwaitable : public TaskAwaitable<void>
	{
	public:
		WriteFileAwaitable(std::string path, std::string text);
	};
}