#pragma once

#include <format>
#include <iostream>
#include "../Time/Stopwatch.h"

namespace Sgl
{
	/// <summary>
	/// A simple benchmarking utility that measures the average execution time of a callable action over a specified number of iterations.
	/// </summary>
	class Benchmark
	{
	public:
		/// <summary>
		/// Constructs a benchmark with the given name. Uses a default number of iterations (1000) unless changed via Loops().
		/// </summary>
		/// <param name="name"> - the descriptive name of the benchmark, shown in the output.</param>
		explicit Benchmark(std::string name):
			_name(std::move(name)),
			_loopsNumber(DefaultLoopsNumber)
		{}

		/// <summary>
		/// Sets the number of times the action will be executed during benchmarking.
		/// The value is clamped to at least 1 to ensure at least one run.
		/// </summary>
		/// <param name="value"> - desired number of iterations.</param>
		/// <returns>A reference to this Benchmark instance for method chaining.</returns>
		Benchmark& Loops(size_t value)
		{
			if(value == 0)
			{
				value = 1;
			}

			_loopsNumber = value;
			return *this;
		}

		/// <summary>
		/// Executes the provided action repeatedly (as configured by Loops()), measures total elapsed time,
		/// computes the average per-iteration duration, and prints the result as "{name}: {average_time}".
		/// The action must be invocable with no arguments.
		/// </summary>
		/// <param name="action"> - the code to benchmark.</param>
		template<std::invocable TAction> 
		void Run(TAction action) const
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
		constexpr static size_t DefaultLoopsNumber = 1000;

		std::string _name;
		size_t _loopsNumber;
	};
}