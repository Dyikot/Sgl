#pragma once

#include <format>
#include <iostream>
#include "Time/Stopwatch.h"
#include "Delegate.h"

namespace Sgl
{
	class Benchmark
	{
	private:
		constexpr static size_t DefaultLoopsNumber = 1e2;
		constexpr static auto DefaultName = "Benchmark";

		std::string _name;
		size_t _loopsNumber;
	public:
		Benchmark(std::string name = DefaultName, size_t loopsNumber = DefaultLoopsNumber):
			_name(std::move(name)),
			_loopsNumber(loopsNumber)
		{}

		Benchmark(const Benchmark&) = delete;
		Benchmark(Benchmark&&) = delete;

		template<typename... TArgs> 
		void Run(CAction<TArgs...> auto&& action, TArgs&&... args)
		{
			Stopwatch stopwatch;
			stopwatch.Start();

			for(size_t i = 0; i < _loopsNumber; i++)
			{
				std::invoke(action, std::forward<TArgs>(args)...);
			}

			stopwatch.Pause();
			auto elapsed = stopwatch.Elapsed() / _loopsNumber;
			std::cout << std::format("{}: {}\n", _name, elapsed.ToString());
		}
	};
}