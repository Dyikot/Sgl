#pragma once

namespace Sgl
{
	class TimeSpan
	{
	private:
		float _milliseconds;
	public:
		constexpr explicit TimeSpan(float milliseconds):
			_milliseconds(milliseconds)
		{}

		constexpr float Milliseconds() const { return _milliseconds; }
		constexpr float Seconds() const { return _milliseconds / 1000; }
		constexpr float Microseconds() const { return _milliseconds * 1000; }

		constexpr operator unsigned int() const { return _milliseconds; }

		constexpr TimeSpan& operator+(TimeSpan other)
		{
			_milliseconds += other._milliseconds;
			return *this;
		}

		constexpr TimeSpan& operator-(TimeSpan other)
		{
			_milliseconds -= other._milliseconds;
			return *this;
		}

		constexpr TimeSpan& operator*(TimeSpan other)
		{
			_milliseconds *= other._milliseconds;
			return *this;
		}

		constexpr TimeSpan& operator/(TimeSpan other)
		{
			_milliseconds /= other._milliseconds;
			return *this;
		}

		constexpr TimeSpan& operator+=(TimeSpan other)
		{
			_milliseconds += other._milliseconds;
			return *this;
		}

		constexpr TimeSpan& operator-=(TimeSpan other)
		{
			_milliseconds -= other._milliseconds;
			return *this;
		}

		constexpr TimeSpan& operator*=(TimeSpan other)
		{
			_milliseconds *= other._milliseconds;
			return *this;
		}

		constexpr TimeSpan& operator/=(TimeSpan other)
		{
			_milliseconds /= other._milliseconds;
			return *this;
		}

		constexpr bool operator>(TimeSpan other) const
		{
			return _milliseconds > other._milliseconds;
		}

		constexpr bool operator<(TimeSpan other) const
		{
			return !operator>(other);
		}

		constexpr bool operator>=(TimeSpan other) const
		{
			return _milliseconds >= other._milliseconds;
		}

		constexpr bool operator<=(TimeSpan other) const
		{
			return !operator>=(other);
		}

		constexpr bool operator==(TimeSpan other) const
		{
			return _milliseconds == other._milliseconds;
		}

		constexpr bool operator!=(TimeSpan other) const
		{
			return !operator==(other);
		}
	};
}