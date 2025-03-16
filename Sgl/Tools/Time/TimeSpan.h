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
			return TimeSpan(value * 1e3f);
		}

		static constexpr TimeSpan FromMilliseconds(long long value)
		{
			return TimeSpan(value * 1e6f);
		}

		static constexpr TimeSpan FromSeconds(long long value)
		{
			return TimeSpan(value * 1e9f);
		}

		static constexpr TimeSpan FromMinutes(long long value)
		{
			return TimeSpan(value * 60e9f);
		}

		static constexpr TimeSpan FromHours(long long value)
		{
			return TimeSpan(value * 3600e9f);
		}

		static constexpr TimeSpan Zero() { return TimeSpan(0); }

		constexpr long long Nanoseconds() const { return _nanoseconds; }
		constexpr double Microseconds() const { return _nanoseconds / 1e3; }
		constexpr double Milliseconds() const { return _nanoseconds / 1e6; }
		constexpr double Seconds() const { return _nanoseconds / 1e9; }
		constexpr double Minutes() const { return _nanoseconds / 60e9; }
		constexpr double Hours() const { return _nanoseconds / 3600e9; }

		std::string ToString() const
		{
			if(Hours() > 10)
			{
				return std::format("{:.2f} hr", Hours());
			}
			else if(Minutes() > 10)
			{
				return std::format("{:.2f} min", Minutes());
			}
			else if(Seconds() > 10)
			{
				return std::format("{:.2f} sec", Seconds());
			}
			else if(Milliseconds() > 10)
			{
				return std::format("{:.2f} ms", Milliseconds());
			}
			else if(Microseconds() > 10)
			{
				return std::format("{:.2f} us", Microseconds());
			}
			else
			{
				return std::format("{} ns", Nanoseconds());
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