#pragma once

#include <algorithm>
#include <compare>

namespace Sgl
{
	/// <summary>
	/// Represents an audio volume level, normalized to a safe range (typically [0.0, 1.0]).
	/// </summary>
	class Volume
	{
	private:
		struct MinTag {};
		struct MaxTag {};
	public:
		/// <summary>
		/// Constructs a Volume instance representing the maximum allowed volume.
		/// </summary>
		constexpr Volume(MaxTag) noexcept:
			_value(MaxValue)
		{}

		/// <summary>
		/// Constructs a Volume instance representing the minimum (silent) volume.
		/// </summary>
		constexpr Volume(MinTag) noexcept:
			_value(MinValue)
		{}

		/// <summary>
		/// Constructs a Volume from a raw floating-point value.
		/// The input is automatically clamped to the valid range [MinValue, MaxValue].
		/// </summary>
		/// <param name="value"> - raw volume level; will be clamped if out of bounds.</param>
		constexpr explicit Volume(double value):
			_value(Adjust(value))
		{}

		/// <summary>
		/// Tag used to construct a Volume at its maximum value.
		/// </summary>
		static constexpr MaxTag Max;

		/// <summary>
		/// Tag used to construct a Volume at its minimum (silent) value.
		/// </summary>
		static constexpr MinTag Min;

		friend constexpr Volume operator+(Volume left, Volume right)
		{
			return Volume(left._value + right._value);
		}

		friend constexpr Volume operator-(Volume left, Volume right)
		{
			return Volume(left._value - right._value);
		}

		friend constexpr Volume operator*(Volume left, Volume right)
		{
			return Volume(left._value * right._value);
		}

		friend constexpr Volume operator/(Volume left, Volume right)
		{
			return Volume(left._value / right._value);
		}

		constexpr Volume& operator+=(Volume volume)
		{
			_value = Adjust(_value + volume._value);
			return *this;
		}

		constexpr Volume& operator-=(Volume volume)
		{
			_value = Adjust(_value - volume._value);
			return *this;
		}

		constexpr Volume& operator*=(Volume volume)
		{
			_value *= volume._value;
			return *this;
		}

		constexpr Volume& operator*=(size_t value)
		{
			_value = Adjust(_value * value);
			return *this;
		}

		constexpr Volume& operator/=(size_t value)
		{
			_value = Adjust(_value / value);
			return *this;
		}

		friend constexpr auto operator<=>(Volume, Volume) noexcept = default;
	private:
		static constexpr double MaxValue = 1;
		static constexpr double MinValue = 0;

		static constexpr double Adjust(double value)
		{
			return std::min(std::abs(value), MaxValue);
		}		
	private:
		double _value;
	};
}