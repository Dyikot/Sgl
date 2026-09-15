#pragma once

#include <format>
#include <iostream>
#include "../Time/Stopwatch.h"

namespace Sgl
{
	//! @brief A simple benchmarking utility that measures the average execution time of a callable action over a specified number of iterations
	class Benchmark
	{
	public:
		//! @brief Constructs a benchmark with the given name
		//! @param name The descriptive name of the benchmark, shown in the output
		explicit Benchmark(std::string name):
			_name(std::move(name)),
			_loopsNumber(1)
		{}

		//! @brief Sets the number of times the action will be executed during benchmarking
		//! @param value Desired number of iterations
		//! @return A reference to this Benchmark instance for method chaining
		Benchmark& Loops(size_t value)
		{
			if(value == 0)
			{
				value = 1;
			}

			_loopsNumber = value;
			return *this;
		}

		//! @brief Executes the provided action repeatedly (as configured by Loops()), measures total elapsed time,
		//! computes the average per-iteration duration, and prints the result as "{name}: {average_time}".
		//! The action must be invocable with no arguments.
		//! @param action The code to benchmark
		void Run(std::invocable auto action) const
		{
			Stopwatch stopwatch = Stopwatch::StartNew();

			for(size_t i = 0; i < _loopsNumber; i++)
			{
				action();
			}

			stopwatch.Pause();
			auto elapsed = stopwatch.Elapsed() / static_cast<double>(_loopsNumber);
			std::cout << std::format("{}: {}\n", _name, elapsed.ToString());
		}

	private:
		std::string _name;
		size_t _loopsNumber;
	};
}