#pragma once
#include <SDL/SDL_timer.h>

namespace Sgl
{
	class TimeSpan
	{
	private:
		float _milliseconds;
	public:
		TimeSpan(float milliseconds):
			_milliseconds(milliseconds)
		{}

		float Milliseconds() const  { return _milliseconds; }
		float Seconds() const { return _milliseconds / 1000; }
		float Microseconds() const { return _milliseconds * 1000; }

		operator uint32_t() const { return _milliseconds; }

		TimeSpan& operator+(TimeSpan other)
		{
			_milliseconds += other._milliseconds;
			return *this;
		}

		TimeSpan& operator-(TimeSpan other)
		{
			_milliseconds -= other._milliseconds;
			return *this;
		}

		TimeSpan& operator*(TimeSpan other)
		{
			_milliseconds *= other._milliseconds;
			return *this;
		}

		TimeSpan& operator/(TimeSpan other)
		{
			_milliseconds /= other._milliseconds;
			return *this;
		}

		bool operator>(TimeSpan other) const
		{
			return _milliseconds > other._milliseconds;
		}

		bool operator<(TimeSpan other) const
		{
			return !operator>(other);
		}

		bool operator>=(TimeSpan other) const
		{
			return _milliseconds >= other._milliseconds;
		}

		bool operator<=(TimeSpan other) const
		{
			return !operator>=(other);
		}

		bool operator==(TimeSpan other) const
		{
			return _milliseconds == other._milliseconds;
		}

		bool operator!=(TimeSpan other) const
		{
			return !operator==(other);
		}
	};

	class Stopwatch
	{
	private:
		size_t _start = 0;
	public:
		void Restart() { _start = SDL_GetPerformanceCounter(); }

		TimeSpan Elapsed()
		{
			return 1000.f * static_cast<float>(SDL_GetPerformanceCounter() - _start)
				/ SDL_GetPerformanceFrequency();
		}		
	};
}