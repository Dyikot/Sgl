#pragma once

#include <chrono>
#include <format>
#include <iostream>
#include <functional>

namespace Sgl
{
	template<typename T>
	class Benchmark
	{
	private:
		std::function<T> _function;
		size_t _number = 1;
		std::string _name = "Benchmark";
	public:
		Benchmark(const std::function<T>& function):
			_function(function)
		{}

		Benchmark& Number(size_t number)
		{
			_number = number;
			return *this;
		}

		Benchmark& Name(std::string&& name)
		{
			_name = std::move(name);
			return *this;
		}

		Benchmark& Name(const std::string& name)
		{
			_name = name;
			return *this;
		}

		template<typename... TArgs> requires std::invocable<T, TArgs...>
		void Run(TArgs&&... args)
		{
			using namespace std::chrono;
			using namespace std::chrono_literals;
			
			auto start = high_resolution_clock::now();
			for(size_t i = 0; i < _number; i++)
			{
				_function(std::forward<TArgs>(args)...);
			}
			auto end = high_resolution_clock::now();
			auto average = (end - start) / _number;
			
			Print(TimeSpan(average.count()));
		}
	private:
		void Print(TimeSpan duration)
		{
			std::cout << std::format("{}\t{}\n", _name, duration.ToString());
		}
	};
}