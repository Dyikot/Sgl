#pragma once

#include <format>
#include <iostream>
#include "../Time/Stopwatch.h"

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

		Benchmark& Loops(size_t value)
		{
			if(value == 0)
			{
				value = 1;
			}

			_loopsNumber = value;
			return *this;
		}

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
	};
}