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

		friend constexpr TimeSpan operator+(TimeSpan left, TimeSpan right)
		{
			return TimeSpan(left._milliseconds + right._milliseconds);
		}

		friend constexpr TimeSpan operator-(TimeSpan left, TimeSpan right)
		{
			return TimeSpan(left._milliseconds - right._milliseconds);
		}

		friend constexpr TimeSpan operator*(TimeSpan left, TimeSpan right)
		{
			return TimeSpan(left._milliseconds * right._milliseconds);
		}

		friend constexpr TimeSpan operator/(TimeSpan left, TimeSpan right)
		{
			return TimeSpan(left._milliseconds / right._milliseconds);
		}

		friend constexpr auto operator<=>(TimeSpan left, TimeSpan right)
		{
			return left._milliseconds <=> right._milliseconds;
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
	};
}