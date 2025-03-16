#pragma once

#include <chrono>
#include <format>
#include <iostream>
#include <functional>

namespace Sgl
{
	class Benchmark
	{
	private:
		size_t _loops = 1;
		std::string _name = "Benchmark";
	public:
		static Benchmark New() { return Benchmark(); }

		Benchmark& SetLoopNumber(size_t number)
		{
			_loops = number;
			return *this;
		}

		Benchmark& SetName(const std::string& name)
		{
			_name = name;
			return *this;
		}

		template<typename TInvocable, typename... TArgs> 
			requires std::invocable<TInvocable, TArgs...>
		void Run(TInvocable&& invocable, TArgs&&... args)
		{
			Stopwatch stopwatch;
			stopwatch.Start();

			for(size_t i = 0; i < _loops; i++)
			{
				std::invoke(invocable, std::forward<TArgs>(args)...);
			}

			stopwatch.Pause();
			auto elapsed = stopwatch.Elapsed() / _loops;
			std::cout << std::format("{}: {}\n", _name, elapsed.ToString());
		}
	private:
		Benchmark() = default;
		Benchmark(const Benchmark&) = delete;
		Benchmark(Benchmark&&) = delete;

		void Print(TimeSpan duration)
		{
			std::cout << std::format("{}: {}\n", _name, duration.ToString());
		}
	};
}