#pragma once

#include <ranges>
#include <future>
#include <type_traits>

namespace Sgl
{
	template<class T>
	struct is_future: std::false_type {};

	template<class T>
	struct is_future<std::future<T>>: std::true_type {};

	template<class T>
	constexpr bool is_future_v = is_future<T>::value;

	class Task
	{
	public:
		template<typename TInvocable, typename... TArgs> 
			requires CAction<TInvocable, TArgs...>
		static auto Run(TInvocable&& invocable, TArgs&&... args)
		{
			return std::async(std::launch::async,
							  std::forward<TInvocable>(invocable),
							  std::forward<TArgs>(args)...);
		}

		template<typename TRange> requires is_future_v<std::ranges::range_value_t<TRange>>
		static void WaitAll(TRange&& tasks)
		{
			for(const auto& task : tasks)
			{
				task.wait();
			}
		}
	};
}