#pragma once

#include <algorithm>
#include <compare>

namespace Sgl
{
	class Volume
	{
	private:
		struct MinTag {};
		struct MaxTag {};

		static constexpr double MaxValue = 1;
		static constexpr double MinValue = 0;

		double _value;
	public:
		static constexpr MaxTag Max;
		static constexpr MinTag Min;
	public:
		constexpr Volume(MaxTag) noexcept:
			_value(MaxValue)
		{}

		constexpr Volume(MinTag) noexcept:
			_value(MinValue)
		{}

		constexpr explicit Volume(double value):
			_value(Adjust(value))
		{}		

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
		static constexpr double Adjust(double value)
		{
			return std::min(std::abs(value), MaxValue);
		}
	};
}