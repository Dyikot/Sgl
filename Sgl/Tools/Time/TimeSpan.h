#pragma once

#include <format>

namespace Sgl
{
	class TimeSpan
	{
	private:
		long long _nanoseconds;
	public:
		constexpr explicit TimeSpan(long long nanoseconds):
			_nanoseconds(nanoseconds)
		{}

		constexpr TimeSpan(const TimeSpan& timespan):
			_nanoseconds(timespan._nanoseconds)
		{}

		static constexpr TimeSpan FromMicroseconds(long long value)
		{
			return TimeSpan(value * 1e3);
		}

		static constexpr TimeSpan FromMilliseconds(long long value)
		{
			return TimeSpan(value * 1e6);
		}

		static constexpr TimeSpan FromSeconds(long long value)
		{
			return TimeSpan(value * 1e9);
		}

		static constexpr TimeSpan FromMinutes(long long value)
		{
			return TimeSpan(value * 6e10);
		}

		static constexpr TimeSpan FromHours(long long value)
		{
			return TimeSpan(value * 36e11);
		}

		static constexpr TimeSpan FromDays(long long value)
		{
			return TimeSpan(value * 864e11);
		}

		static constexpr TimeSpan Zero()
		{
			return TimeSpan(0);
		}

		constexpr auto ToNanoseconds() const { return _nanoseconds; }
		constexpr auto ToMicroseconds() const { return _nanoseconds / 1e3; }
		constexpr auto ToMilliseconds() const { return _nanoseconds / 1e6; }
		constexpr auto ToSeconds() const { return _nanoseconds / 1e9; }
		constexpr auto ToMinutes() const { return _nanoseconds / 6e10; }
		constexpr auto ToHours() const { return _nanoseconds / 36e11; }
		constexpr auto ToDays() const { return _nanoseconds / 864e11; }

		constexpr TimeSpan Duration() const
		{
			return TimeSpan(_nanoseconds > 0 ? _nanoseconds : -_nanoseconds);
		}

		std::string ToString() const;

		friend constexpr TimeSpan operator+(TimeSpan left, TimeSpan right)
		{
			return TimeSpan(left._nanoseconds + right._nanoseconds);
		}

		friend constexpr TimeSpan operator-(TimeSpan left, TimeSpan right)
		{
			return TimeSpan(left._nanoseconds - right._nanoseconds);
		}

		friend constexpr TimeSpan operator*(TimeSpan left, TimeSpan right)
		{
			return TimeSpan(left._nanoseconds * right._nanoseconds);
		}

		friend constexpr TimeSpan operator/(TimeSpan left, TimeSpan right)
		{
			return TimeSpan(left._nanoseconds / right._nanoseconds);
		}

		friend constexpr TimeSpan operator*(TimeSpan timespan, double number)
		{
			return TimeSpan(timespan._nanoseconds * number);
		}

		friend constexpr TimeSpan operator/(TimeSpan timespan, double number)
		{
			return TimeSpan(timespan._nanoseconds / number);
		}

		friend constexpr TimeSpan operator+(TimeSpan other)
		{
			return other;
		}

		friend constexpr TimeSpan operator-(TimeSpan other)
		{
			return TimeSpan(-other._nanoseconds);
		}

		friend constexpr auto operator<=>(TimeSpan left, TimeSpan right)
		{
			return left._nanoseconds <=> right._nanoseconds;
		}

		constexpr TimeSpan& operator+=(TimeSpan other)
		{
			_nanoseconds += other._nanoseconds;
			return *this;
		}

		constexpr TimeSpan& operator-=(TimeSpan other)
		{
			_nanoseconds -= other._nanoseconds;
			return *this;
		}

		constexpr TimeSpan& operator*=(TimeSpan other)
		{
			_nanoseconds *= other._nanoseconds;
			return *this;
		}

		constexpr TimeSpan& operator/=(TimeSpan other)
		{
			_nanoseconds /= other._nanoseconds;
			return *this;
		}

		constexpr TimeSpan& operator*=(double number)
		{
			_nanoseconds *= number;
			return *this;
		}

		constexpr TimeSpan& operator/=(double number)
		{
			_nanoseconds /= number;
			return *this;
		}
	};
}