#pragma once

#include <format>
#include <numeric>
#include <compare>

namespace Sgl
{
	class TimeSpan
	{
	public:
		constexpr explicit TimeSpan(int64_t nanoseconds) noexcept:
			_nanoseconds(nanoseconds)
		{}

		constexpr TimeSpan(const TimeSpan& timespan) noexcept:
			_nanoseconds(timespan._nanoseconds)
		{}

		constexpr TimeSpan(TimeSpan&& timespan) noexcept:
			_nanoseconds(timespan._nanoseconds)
		{}

		/// <summary>
		/// Represents a zero time interval (0 nanoseconds).
		/// </summary>
		static const TimeSpan Zero;

		/// <summary>
		/// Represents the maximum possible time interval.
		/// </summary>
		static const TimeSpan MaxValue;

		/// <summary>
		/// Represents the minimum possible time interval.
		/// </summary>
		static const TimeSpan MinValue;

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

		constexpr int64_t GetNanoseconds() const noexcept
		{
			return _nanoseconds;
		}

		constexpr int64_t GetMicrosecond() const noexcept
		{
			return static_cast<int64_t>(_nanoseconds / NanosecondsPerDay) % 1000;
		}

		constexpr int64_t GetMilliseconds() const noexcept
		{
			return static_cast<int64_t>(ToMilliseconds()) % 1000;
		}

		constexpr int64_t GetSeconds() const noexcept
		{
			return static_cast<int64_t>(ToSeconds()) % 60;
		}

		constexpr int64_t GetMinutes() const noexcept
		{
			return static_cast<int64_t>(ToMinutes()) % 60;
		}

		constexpr int64_t GetHours() const noexcept
		{
			return static_cast<int64_t>(ToHours()) % 24;
		}

		constexpr int64_t GetDays() const noexcept
		{
			return static_cast<int64_t>(ToDays());
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

		constexpr TimeSpan& operator=(const TimeSpan& other) noexcept
		{
			_nanoseconds = other._nanoseconds;
			return *this;
		}

		constexpr TimeSpan& operator=(TimeSpan&& other) noexcept
		{
			_nanoseconds = other._nanoseconds;
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

		static constexpr int64_t ThresholdMicroseconds = 2 * NanosecondsPerMicrosecond;
		static constexpr int64_t ThresholdMilliseconds = 2 * NanosecondsPerMillisecond;
		static constexpr int64_t ThresholdSeconds = 2 * NanosecondsPerSecond;
		static constexpr int64_t ThresholdMinutes = 2 * NanosecondsPerMinute;
		static constexpr int64_t ThresholdHours = 2 * NanosecondsPerHour;
		static constexpr int64_t ThresholdDays = 2 * NanosecondsPerDay;

		int64_t _nanoseconds;
	};

	inline constexpr TimeSpan TimeSpan::Zero { 0 };
	inline constexpr TimeSpan TimeSpan::MaxValue { std::numeric_limits<int64_t>::max() };
	inline constexpr TimeSpan TimeSpan::MinValue { std::numeric_limits<int64_t>::min() };
}