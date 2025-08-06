#pragma once

#include <string_view>
#include <SDL/SDL_mixer.h>
#include "../Base/Log.h"
#include "../Base/Time/TimeSpan.h"
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

		friend constexpr auto operator<=>(Volume, Volume) noexcept = default;

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
		Volume Volume = Volume::Min;
	public:
		explicit Music(std::string_view path) noexcept;
		Music(const Music&) = delete;
		Music(Music&& other) noexcept;
		~Music() noexcept;

		Mix_Music* GetMixMusic() const noexcept;

		void Play(int loops = 0) const;
		void Pause() noexcept;
		void Resume() noexcept;
		void Rewind() noexcept;
		void Halt() noexcept;
		bool IsPaused() const noexcept;
		bool IsPlaying() const noexcept;
		TimeSpan Duration() const noexcept;

		Music& operator=(Music&& other) noexcept;
	};

	class SoundChunk
	{
	private:
		Mix_Chunk* _soundChunk;
	public:
		Volume Volume = Volume::Min;
	public:
		explicit SoundChunk(std::string_view path) noexcept;
		SoundChunk(const SoundChunk&) = delete;
		SoundChunk(SoundChunk&& other) noexcept;
		~SoundChunk() noexcept;

		Mix_Chunk* GetMixChunk() const noexcept;

		void Play(int loops = 0);
		void Play(int channel, int loops = 0) const;

		SoundChunk& operator=(SoundChunk&& other) noexcept;
	};
}