#pragma once

#include <future>

namespace Sgl
{
	class Task
	{
	public:
		template<typename TCallable, typename... TArgs>
		static auto Run(TCallable&& callable, TArgs&&... args)
		{
			return std::async(std::launch::async,
							  std::forward<TCallable>(callable),
							  std::forward<TArgs>(args)...);
		}
	};
}