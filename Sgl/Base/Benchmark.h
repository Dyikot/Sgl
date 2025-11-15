#pragma once

#include <format>
#include <iostream>
#include "Time/Stopwatch.h"

namespace Sgl
{
	class Benchmark
	{
	private:
		constexpr static size_t DefaultLoopsNumber = 1000;

		std::string _name;
		size_t _loopsNumber;
	public:
		explicit Benchmark(std::string name):
			_name(std::move(name)),
			_loopsNumber(DefaultLoopsNumber)
		{}

		Benchmark(const Benchmark&) = delete;
		Benchmark(Benchmark&&) = delete;

		Benchmark& Loops(size_t value)
		{
			_loopsNumber = value;
			return *this;
		}

		template<std::invocable TAction> 
		void Run(TAction action)
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
	};
}