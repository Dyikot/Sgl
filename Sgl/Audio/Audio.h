#pragma once

#include <string_view>
#include "SDL/SDL_mixer.h"
#include "../Base/Log.h"
#include "../Base/Time/TimeSpan.h"

namespace Sgl
{
	class Volume
	{
	private:
		static constexpr double MaxValue = 1;
		static constexpr double MinValue = 0;
		double _value;
	public:
		static constexpr Volume Min() { return Volume(MinValue); }
		static constexpr Volume Max() { return Volume(MaxValue); }

		constexpr Volume() noexcept:
			_value(MaxValue)
		{}

		constexpr explicit Volume(double value):
			_value(Adjust(value > MinValue ? value : -value))
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
			return std::min(value, MaxValue);
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
		explicit Music(std::string_view path) noexcept;
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

		operator Mix_Music* () const noexcept { return _music; }
	};

	class SoundChunk
	{
	private:
		Mix_Chunk* _soundChunk;
	public:
		Volume Volume;
	public:
		explicit SoundChunk(std::string path) noexcept;
		SoundChunk(const SoundChunk&) = delete;
		SoundChunk(SoundChunk&&) = delete;
		~SoundChunk() noexcept;

		void Play(int loops = 0);
		void Play(int channel, int loops = 0) const;

		operator Mix_Chunk* () const noexcept { return _soundChunk; }
	};
}