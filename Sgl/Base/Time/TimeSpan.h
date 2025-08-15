#pragma once

#include <format>
#include <numeric>
#include <compare>

namespace Sgl
{
	class TimeSpan
	{
	private:
		struct TimeSpanZero {};
		struct TimeSpanMax {};
		struct TimeSpanMin {};
	public:
		constexpr static TimeSpanZero Zero;
		constexpr static TimeSpanMax MaxValue;
		constexpr static TimeSpanMin MinValue;
	private:
		static constexpr double NanosecondsPerMicrosecond = 1e3;
		static constexpr double NanosecondsPerMillisecond = 1e6;
		static constexpr double NanosecondsPerSecond = 1e9;
		static constexpr double NanosecondsPerMinute = 6e10;
		static constexpr double NanosecondsPerHour = 36e11;
		static constexpr double NanosecondsPerDay = 864e11;

		static constexpr double ToMicrosecondsCoefficient = 1 / NanosecondsPerMicrosecond;
		static constexpr double ToMillisecondsCoefficient = 1 / NanosecondsPerMillisecond;
		static constexpr double ToSecondsCoefficient = 1 / NanosecondsPerSecond;
		static constexpr double ToMinutesCoefficient = 1 / NanosecondsPerMinute;
		static constexpr double ToHoursCoefficient = 1 / NanosecondsPerHour;
		static constexpr double ToDaysCoefficient = 1 / NanosecondsPerDay;

		static constexpr long long ThresholdMicroseconds = 10 * NanosecondsPerMicrosecond;
		static constexpr long long ThresholdMilliseconds = 10 * NanosecondsPerMillisecond;
		static constexpr long long ThresholdSeconds = 10 * NanosecondsPerSecond;
		static constexpr long long ThresholdMinutes = 10 * NanosecondsPerMinute;
		static constexpr long long ThresholdHours = 10 * NanosecondsPerHour;
		static constexpr long long ThresholdDays = 10 * NanosecondsPerDay;

		long long _nanoseconds;
	public:
		constexpr TimeSpan(TimeSpanZero):
			_nanoseconds()
		{}

		constexpr TimeSpan(TimeSpanMax):
			_nanoseconds(std::numeric_limits<long long>::max())
		{}

		constexpr TimeSpan(TimeSpanMin):
			_nanoseconds(std::numeric_limits<long long>::min())
		{}

		constexpr explicit TimeSpan(long long nanoseconds) noexcept:
			_nanoseconds(nanoseconds)
		{}

		constexpr TimeSpan(const TimeSpan& timespan) noexcept:
			_nanoseconds(timespan._nanoseconds)
		{}

		static constexpr TimeSpan FromMicroseconds(long long value) noexcept
		{
			return TimeSpan(value * static_cast<long long>(NanosecondsPerMicrosecond));
		}

		static constexpr TimeSpan FromMilliseconds(long long value) noexcept
		{
			return TimeSpan(value * static_cast<long long>(NanosecondsPerMillisecond));
		}

		static constexpr TimeSpan FromSeconds(long long value) noexcept
		{
			return TimeSpan(value * static_cast<long long>(NanosecondsPerSecond));
		}

		static constexpr TimeSpan FromMinutes(long long value) noexcept
		{
			return TimeSpan(value * static_cast<long long>(NanosecondsPerMinute));
		}

		static constexpr TimeSpan FromHours(long long value) noexcept
		{
			return TimeSpan(value * static_cast<long long>(NanosecondsPerHour));
		}

		static constexpr TimeSpan FromDays(long long value) noexcept
		{
			return TimeSpan(value * static_cast<long long>(NanosecondsPerDay));
		}

		constexpr long long ToNanoseconds() const noexcept
		{
			return _nanoseconds;
		}

		constexpr double ToMicroseconds() const noexcept
		{
			return _nanoseconds * ToMicrosecondsCoefficient;
		}

		constexpr double ToMilliseconds() const noexcept
		{ 
			return _nanoseconds * ToMillisecondsCoefficient;
		}

		constexpr double ToSeconds() const noexcept
		{ 
			return _nanoseconds * ToSecondsCoefficient; 
		}

		constexpr double ToMinutes() const noexcept
		{ 
			return _nanoseconds * ToMinutesCoefficient;
		}

		constexpr double ToHours() const noexcept
		{ 
			return _nanoseconds * ToHoursCoefficient;
		}

		constexpr double ToDays() const noexcept 
		{ 
			return _nanoseconds * ToDaysCoefficient;
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

		friend constexpr auto operator<=>(TimeSpan, TimeSpan) noexcept = default;
	};
}