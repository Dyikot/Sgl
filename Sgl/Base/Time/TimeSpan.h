#pragma once

#include <format>

namespace Sgl
{
	class TimeSpan
	{
	private:
		static constexpr double NanosecondsPerMicrosecond = 1e3;
		static constexpr double NanosecondsPerMillisecond = 1e6;
		static constexpr double NanosecondsPerSecond = 1e9;
		static constexpr double NanosecondsPerMinute = 6e10;
		static constexpr double NanosecondsPerHour = 36e11;
		static constexpr double NanosecondsPerDay = 864e11;

		static constexpr long long ThresholdMicroseconds = 10 * NanosecondsPerMicrosecond;
		static constexpr long long ThresholdMilliseconds = 10 * NanosecondsPerMillisecond;
		static constexpr long long ThresholdSeconds = 10 * NanosecondsPerSecond;
		static constexpr long long ThresholdMinutes = 10 * NanosecondsPerMinute;
		static constexpr long long ThresholdHours = 10 * NanosecondsPerHour;
		static constexpr long long ThresholdDays = 10 * NanosecondsPerHour;

		long long _nanoseconds;
	public:
		constexpr explicit TimeSpan(long long nanoseconds) noexcept:
			_nanoseconds(nanoseconds)
		{}

		constexpr TimeSpan(const TimeSpan& timespan) noexcept:
			_nanoseconds(timespan._nanoseconds)
		{}

		static constexpr TimeSpan FromMicroseconds(long long value) noexcept
		{
			return TimeSpan(value * NanosecondsPerMicrosecond);
		}

		static constexpr TimeSpan FromMilliseconds(long long value) noexcept
		{
			return TimeSpan(value * NanosecondsPerMillisecond);
		}

		static constexpr TimeSpan FromSeconds(long long value) noexcept
		{
			return TimeSpan(value * NanosecondsPerSecond);
		}

		static constexpr TimeSpan FromMinutes(long long value) noexcept
		{
			return TimeSpan(value * NanosecondsPerMinute);
		}

		static constexpr TimeSpan FromHours(long long value) noexcept
		{
			return TimeSpan(value * NanosecondsPerHour);
		}

		static constexpr TimeSpan FromDays(long long value) noexcept
		{
			return TimeSpan(value * NanosecondsPerDay);
		}

		static constexpr TimeSpan Zero() noexcept
		{
			return TimeSpan(0);
		}

		constexpr long long ToNanoseconds() const noexcept
		{
			return _nanoseconds;
		}

		constexpr double ToMicroseconds() const noexcept
		{
			return _nanoseconds / NanosecondsPerMicrosecond;
		}

		constexpr double ToMilliseconds() const noexcept
		{ 
			return _nanoseconds / NanosecondsPerMillisecond;
		}

		constexpr double ToSeconds() const noexcept
		{ 
			return _nanoseconds / NanosecondsPerSecond; 
		}

		constexpr double ToMinutes() const noexcept
		{ 
			return _nanoseconds / NanosecondsPerMinute;
		}

		constexpr double ToHours() const noexcept
		{ 
			return _nanoseconds / NanosecondsPerHour;
		}

		constexpr double ToDays() const noexcept 
		{ 
			return _nanoseconds / NanosecondsPerDay;
		}

		constexpr TimeSpan Duration() const noexcept
		{
			return TimeSpan(std::abs(_nanoseconds));
		}

		std::string ToString() const;

		friend constexpr TimeSpan operator+(TimeSpan left, TimeSpan right) noexcept
		{
			return TimeSpan(left._nanoseconds + right._nanoseconds);
		}

		friend constexpr TimeSpan operator-(TimeSpan left, TimeSpan right) noexcept
		{
			return TimeSpan(left._nanoseconds - right._nanoseconds);
		}

		friend constexpr TimeSpan operator*(TimeSpan left, TimeSpan right) noexcept
		{
			return TimeSpan(left._nanoseconds * right._nanoseconds);
		}

		friend constexpr TimeSpan operator/(TimeSpan left, TimeSpan right) noexcept
		{
			return TimeSpan(left._nanoseconds / right._nanoseconds);
		}

		friend constexpr TimeSpan operator*(TimeSpan timespan, double number) noexcept
		{
			return TimeSpan(timespan._nanoseconds * number);
		}

		friend constexpr TimeSpan operator/(TimeSpan timespan, double number) noexcept
		{
			return TimeSpan(timespan._nanoseconds / number);
		}

		friend constexpr TimeSpan operator+(TimeSpan other) noexcept
		{
			return other;
		}

		friend constexpr TimeSpan operator-(TimeSpan other) noexcept
		{
			return TimeSpan(-other._nanoseconds);
		}

		friend constexpr auto operator<=>(TimeSpan left, TimeSpan right) noexcept
		{
			return left._nanoseconds <=> right._nanoseconds;
		}

		constexpr TimeSpan& operator+=(TimeSpan other) noexcept
		{
			_nanoseconds += other._nanoseconds;
			return *this;
		}

		constexpr TimeSpan& operator-=(TimeSpan other) noexcept
		{
			_nanoseconds -= other._nanoseconds;
			return *this;
		}

		constexpr TimeSpan& operator*=(TimeSpan other) noexcept
		{
			_nanoseconds *= other._nanoseconds;
			return *this;
		}

		constexpr TimeSpan& operator/=(TimeSpan other) noexcept
		{
			_nanoseconds /= other._nanoseconds;
			return *this;
		}

		constexpr TimeSpan& operator*=(double number) noexcept
		{
			_nanoseconds *= number;
			return *this;
		}

		constexpr TimeSpan& operator/=(double number) noexcept
		{
			_nanoseconds /= number;
			return *this;
		}
	};
}