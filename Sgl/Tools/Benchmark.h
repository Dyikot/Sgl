#pragma once

#include <chrono>
#include <format>
#include <iostream>
#include <functional>

namespace Sgl
{
	template<typename TFunction>
	class Benchmark
	{
	private:
		std::function<TFunction> _function;
		size_t _number = 1;
		std::string _name = "Benchmark";
	public:
		Benchmark(TFunction&& function):
			_function(std::forward<TFunction>(function))
		{}
		template <typename TInvokable>
		Benchmark(TInvokable&& function):
			_function(std::forward<TInvokable>(function))
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

		template<typename... TArgs> requires std::invocable<TFunction, TArgs...>
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
			auto avg = (end - start) / _number;

			if(auto result = duration_cast<nanoseconds>(avg); result < 1000ns)
			{
				Print(result);
			}
			else if(auto result = duration_cast<microseconds>(avg); result < 1000us)
			{
				Print(result);
			}
			else if(auto result = duration_cast<milliseconds>(avg); result < 1000ms)
			{
				Print(result);
			}
			else
			{
				Print(duration_cast<seconds>(avg));
			}
		}
	private:
		void Print(auto&& duration)
		{
			std::cout << std::format("{}\t{}\n", _name, duration);
		}
	};
}