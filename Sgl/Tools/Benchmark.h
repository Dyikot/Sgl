#pragma once

#include <format>
#include <iostream>
#include "Time/Stopwatch.h"

namespace Sgl
{
	class Benchmark
	{
	private:
		size_t _loops = 1;
		std::string _name = "Benchmark";
	public:
		static Benchmark New() { return Benchmark(); }
		static Benchmark New(size_t loops) { return Benchmark("Benchmark", loops); }
		static Benchmark New(const std::string& name) { return Benchmark(name, 1); }
		static Benchmark New(const std::string& name, size_t loops) { return Benchmark(name, loops); }

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

		Benchmark(const std::string& name, size_t loops):
			_name(name), _loops(loops)
		{}		
	};
}