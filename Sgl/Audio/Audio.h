#pragma once

#include <string_view>
#include "SDL/SDL_mixer.h"
#include "../Tools/Log.h"
#include "../Tools/Time/TimeSpan.h"

namespace Sgl
{
	class Volume
	{
	public:
		static constexpr Volume Max() { return Volume(1); }
		static constexpr Volume Zero() { return Volume(0); }
	private:
		static constexpr double MaxValue = 1;
		double _value;
	public:
		constexpr Volume() noexcept:
			_value(MaxValue)
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
		Music(std::string_view path) noexcept;
		Music(const Music&) = delete;
		Music(Music&&) = delete;
		~Music() noexcept { Mix_FreeMusic(_music); }

		void Play(int loops = 0) const;
		void Pause() noexcept;
		void Resume() noexcept { Mix_ResumeMusic(); }
		void Rewind() noexcept { Mix_RewindMusic(); }
		void Halt() noexcept { Mix_HaltMusic(); }
		bool IsPaused() noexcept { return Mix_PausedMusic(); }
		bool IsPlaying() noexcept { return Mix_PlayingMusic(); }
		Mix_Music* ToMix_Music() const noexcept { return _music; }
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
		~SoundChunk() noexcept { Mix_FreeChunk(_soundChunk); }

		void Play(int channel = Auto, int loops = 0) const;
		Mix_Chunk* ToMix_Chunk() const noexcept { return _soundChunk; }
	};

	using MusicView = std::reference_wrapper<Music>;
	using SoundChunkView = std::reference_wrapper<SoundChunk>;
}