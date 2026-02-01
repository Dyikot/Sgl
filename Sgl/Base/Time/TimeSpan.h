#pragma once

#include <format>
#include <numeric>
#include <compare>

namespace Sgl
{
	class TimeSpan
	{
	private:
		struct ZeroTag {};
		struct MaxTag {};
		struct MinTag {};
	public:
		constexpr TimeSpan(ZeroTag):
			_nanoseconds()
		{}

		constexpr TimeSpan(MaxTag):
			_nanoseconds(std::numeric_limits<int64_t>::max())
		{}

		constexpr TimeSpan(MinTag):
			_nanoseconds(std::numeric_limits<int64_t>::min())
		{}

		constexpr explicit TimeSpan(int64_t nanoseconds) noexcept:
			_nanoseconds(nanoseconds)
		{}

		constexpr TimeSpan(const TimeSpan& timespan) noexcept:
			_nanoseconds(timespan._nanoseconds)
		{}

		/// <summary>
		/// Represents a zero time interval (0 nanoseconds).
		/// </summary>
		constexpr static ZeroTag Zero;

		/// <summary>
		/// Represents the maximum possible time interval.
		/// </summary>
		constexpr static MaxTag MaxValue;

		/// <summary>
		/// Represents the minimum possible time interval.
		/// </summary>
		constexpr static MinTag MinValue;

		static constexpr TimeSpan FromMicroseconds(int64_t value) noexcept
		{
			return TimeSpan(value * static_cast<int64_t>(NanosecondsPerMicrosecond));
		}

		static constexpr TimeSpan FromMilliseconds(int64_t value) noexcept
		{
			return TimeSpan(value * static_cast<int64_t>(NanosecondsPerMillisecond));
		}

		static constexpr TimeSpan FromSeconds(int64_t value) noexcept
		{
			return TimeSpan(value * static_cast<int64_t>(NanosecondsPerSecond));
		}

		static constexpr TimeSpan FromMinutes(int64_t value) noexcept
		{
			return TimeSpan(value * static_cast<int64_t>(NanosecondsPerMinute));
		}

		static constexpr TimeSpan FromHours(int64_t value) noexcept
		{
			return TimeSpan(value * static_cast<int64_t>(NanosecondsPerHour));
		}

		static constexpr TimeSpan FromDays(int64_t value) noexcept
		{
			return TimeSpan(value * static_cast<int64_t>(NanosecondsPerDay));
		}

		constexpr int64_t ToNanoseconds() const noexcept
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

		static constexpr int64_t ThresholdMicroseconds = 10 * NanosecondsPerMicrosecond;
		static constexpr int64_t ThresholdMilliseconds = 10 * NanosecondsPerMillisecond;
		static constexpr int64_t ThresholdSeconds = 10 * NanosecondsPerSecond;
		static constexpr int64_t ThresholdMinutes = 10 * NanosecondsPerMinute;
		static constexpr int64_t ThresholdHours = 10 * NanosecondsPerHour;
		static constexpr int64_t ThresholdDays = 10 * NanosecondsPerDay;

		int64_t _nanoseconds;
	};
}