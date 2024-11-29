#pragma once

namespace Sgl
{
	class IProcessed
	{
	public:
		virtual ~IProcessed() = default;

		virtual void Process(float elapsedMs) = 0;
	};
}