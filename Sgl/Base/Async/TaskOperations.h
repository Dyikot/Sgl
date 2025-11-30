#pragma once

#include "AsyncTimeManager.h"

namespace Sgl
{
	class TaskOperations
	{
	public:
		static TimeAwaitable Delay(TimeSpan duration);
		static TimeAwaitable Delay(size_t milliseconds);
	};
}