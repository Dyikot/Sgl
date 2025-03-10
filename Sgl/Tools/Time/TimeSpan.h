#pragma once

#include <format>

namespace Sgl
{
	class TimeSpan
	{
	private:
		float _nanoseconds;
	public:
		constexpr explicit TimeSpan(float nanoseconds):
			_nanoseconds(nanoseconds)
		{}

		static constexpr TimeSpan FromMicroseconds(float value)
		{
			return TimeSpan(value * 1e3f);
		}

		static constexpr TimeSpan FromMilliseconds(float value)
		{
			return TimeSpan(value * 1e6f);
		}

		static constexpr TimeSpan FromSeconds(float value)
		{
			return TimeSpan(value * 1e9f);
		}

		static constexpr TimeSpan FromMinutes(float value)
		{
			return TimeSpan(value * 60e9f);
		}

		static constexpr TimeSpan FromHours(float value)
		{
			return TimeSpan(value * 3600e9f);
		}

		static constexpr TimeSpan Zero() { return TimeSpan(0); }

		constexpr float Nanoseconds() const { return _nanoseconds; }
		constexpr float Microseconds() const { return _nanoseconds / 1e3f; }
		constexpr float Milliseconds() const { return _nanoseconds / 1e6f; }
		constexpr float Seconds() const { return _nanoseconds / 1e9f; }
		constexpr float Minutes() const { return _nanoseconds / 60e9f; }
		constexpr float Hours() const { return _nanoseconds / 3600e9f; }

		std::string ToString() const
		{
			if(Hours() > 10)
			{
				return std::format("{} hr", Hours());
			}
			else if(Minutes() > 10)
			{
				return std::format("{} min", Minutes());
			}
			else if(Seconds() > 10)
			{
				return std::format("{} sec", Seconds());
			}
			else if(Milliseconds() > 10)
			{
				return std::format("{} ms", Milliseconds());
			}
			else if(Microseconds() > 10)
			{
				return std::format("{} us", Microseconds());
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
	};
}