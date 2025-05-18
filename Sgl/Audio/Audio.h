#pragma once

#include <string_view>
#include "SDL/SDL_mixer.h"
#include "../Tools/Log.h"
#include "../Tools/Time/TimeSpan.h"

namespace Sgl
{
	class Volume
	{
	private:
		static constexpr double _maxValue = 1;
		double _value;
	public:
		static constexpr Volume Max()
		{
			return Volume(1);
		}

		static constexpr Volume Zero()
		{
			return Volume(0);
		}

		constexpr Volume() noexcept:
			_value(_maxValue)
		{}

		explicit constexpr Volume(double value):
			_value(Adjust(std::abs(value)))
		{}

		constexpr int ToMixVolume() const noexcept
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

		friend constexpr auto operator<=>(Volume left, Volume right)
		{
			return left._value <=> right._value;
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
	private:
		static constexpr double Adjust(double value)
		{
			return std::min(value, _maxValue);
		}
	};

	class Music
	{
	private:
		Mix_Music* _music;
	public:
		Volume Volume;
		const TimeSpan Duration;
	public:
		Music(std::string_view path) noexcept;
		Music(const Music&) = delete;
		Music(Music&&) = delete;
		~Music() noexcept;

		void Play(int loops = 0) const;
		void Pause() noexcept;
		void Resume() noexcept;
		void Rewind() noexcept;
		void Halt() noexcept;
		bool IsPaused() noexcept;
		bool IsPlaying() noexcept;

		operator Mix_Music* () const noexcept;
	};

	class SoundChunk
	{
	private:
		static constexpr int Auto = -1;

		Mix_Chunk* _soundChunk;
	public:
		Volume Volume;
	public:
		SoundChunk(std::string_view path) noexcept;
		SoundChunk(const SoundChunk&) = delete;
		SoundChunk(SoundChunk&&) = delete;
		~SoundChunk() noexcept;

		void Play(int channel = Auto, int loops = 0) const;

		operator Mix_Chunk* () const noexcept;
	};
}