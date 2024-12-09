#pragma once

#include "../Tools/Stopwatch.h"

namespace Sgl
{
	class IProcessed
	{
	public:
		virtual ~IProcessed() = default;

		virtual void Process(TimeSpan elapsed) = 0;
	};
}