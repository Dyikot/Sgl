#pragma once

#include "../Tools/Time/TimeSpan.h"

namespace Sgl::ECS
{
	class IProcessed
	{
	public:
		virtual ~IProcessed() = default;

		virtual void OnProcessing(TimeSpan elapsed) = 0;
	};
}