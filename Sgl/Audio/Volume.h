#pragma once

#include <SDL/SDL_mixer.h>
#include <algorithm>
#include <compare>

namespace Sgl
{
	class Volume
	{
	private:
		struct VolumeMin {};
		struct VolumeMax {};

		static constexpr double MaxValue = 1;
		static constexpr double MinValue = 0;

		double _value;
	public:
		static constexpr VolumeMax Max;
		static constexpr VolumeMin Min;
	public:
		constexpr Volume(VolumeMax) noexcept:
			_value(MaxValue)
		{}

		constexpr Volume(VolumeMin) noexcept:
			_value(MinValue)
		{}

		constexpr explicit Volume(double value):
			_value(Adjust(std::abs(value)))
		{}

		constexpr explicit Volume(uint8_t mixValue):
			Volume(static_cast<double>(mixValue) / (MIX_MAX_VOLUME))
		{}		

		constexpr uint8_t ToMixVolume() const noexcept
		{
			return _value * MIX_MAX_VOLUME;
		}

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
			return std::min(value, MaxValue);
		}
	};
}