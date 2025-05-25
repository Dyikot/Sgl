#pragma once

#include <format>
#include <iostream>
#include "Time/Stopwatch.h"
#include "../Data/Delegate.h"

namespace Sgl
{
	class Benchmark
	{
	private:
		constexpr static size_t DefaultLoopsNumber = 1e2;

		size_t _loopsNumber = DefaultLoopsNumber;
		std::string _name = "Benchmark";
	public:
		static Benchmark New()
		{
			return Benchmark();
		}

		static Benchmark New(size_t loopsNumber) 
		{
			return Benchmark("Benchmark", loopsNumber);
		}

		static Benchmark New(std::string name) 
		{
			return Benchmark(std::move(name), DefaultLoopsNumber);		
		}

		static Benchmark New(std::string name, size_t loopsNumber)
		{
			return Benchmark(std::move(name), loopsNumber);
		}

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
	private:
		Benchmark() = default;
		Benchmark(const Benchmark&) = delete;
		Benchmark(Benchmark&&) = delete;

		Benchmark(std::string name, size_t loopsNumber):
			_name(std::move(name)), _loopsNumber(loopsNumber)
		{}
	};
}