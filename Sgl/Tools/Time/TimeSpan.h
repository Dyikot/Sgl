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

		constexpr long long ToNanoseconds() const { return _nanoseconds; }
		constexpr double ToMicroseconds() const { return _nanoseconds / 1e3; }
		constexpr double ToMilliseconds() const { return _nanoseconds / 1e6; }
		constexpr double ToSeconds() const { return _nanoseconds / 1e9; }
		constexpr double ToMinutes() const { return _nanoseconds / 6e10; }
		constexpr double ToHours() const { return _nanoseconds / 36e11; }
		constexpr double ToDays() const { return _nanoseconds / 864e11; }

		constexpr TimeSpan Duration() const
		{
			return TimeSpan(_nanoseconds > 0 ? _nanoseconds : -_nanoseconds);
		}

		std::string ToString() const
		{
			if(ToMicroseconds() < 10)
			{
				return std::format("{} ns", ToNanoseconds());
			}
			else if(ToMilliseconds() < 10)
			{
				return std::format("{:.2f} us", ToMicroseconds());
			}
			else if(ToSeconds() < 10)
			{
				return std::format("{:.2f} ms", ToMilliseconds());
			}
			else if(ToMinutes() < 10)
			{
				return std::format("{:.2f} sec", ToSeconds());
			}
			else if(ToHours() < 10)
			{
				return std::format("{:.2f} min", ToMinutes());
			}
			else if(ToDays() < 10)
			{
				return std::format("{:.2f} hr", ToHours());
			}
			else
			{
				return std::format("{:.2f} hr", ToDays());
			}
		}

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