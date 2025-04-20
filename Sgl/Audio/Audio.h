#pragma once

#include <string_view>
#include <iostream>
#include <span>
#include "SDL/SDL_mixer.h"
#include "../Tools/Log.h"

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
		explicit constexpr Volume(double value):
			_value(Adjust(std::abs(value)))
		{}

		constexpr int ToMixVolume() const
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

	class IAudio
	{
	public:	
		virtual ~IAudio() = default;

		virtual void SetVolume(Volume value) = 0;
		virtual Volume GetVolume() const = 0;
	};

	class Music: public IAudio
	{
	private:
		Volume _volume = Volume::Max();
		Mix_Music* _music;
	public:
		Music(std::string_view path) noexcept:
			_music(Mix_LoadMUS(path.data()))
		{
			PrintSDLErrorIf(_music == nullptr);
		}

		Music(const Music&) = delete;
		Music(Music&&) = delete;

		~Music() noexcept
		{ 
			Mix_FreeMusic(_music);
		}

		void SetVolume(Volume value) override { _volume = value; }
		Volume GetVolume() const override { return _volume; }

		operator Mix_Music* () const noexcept { return _music; }
	};

	class SoundEffect: public IAudio
	{
	private:
		Volume _volume = Volume::Max();
		Mix_Chunk* _soundChunk;
	public:
		SoundEffect(std::string_view path) noexcept:
			_soundChunk(Mix_LoadWAV(path.data()))
		{
			PrintSDLErrorIf(_soundChunk == nullptr);
		}

		SoundEffect(const SoundEffect&) = delete;
		SoundEffect(SoundEffect&&) = delete;

		~SoundEffect() noexcept
		{ 
			Mix_FreeChunk(_soundChunk);
		}

		void SetVolume(Volume value) override { _volume = value; }
		Volume GetVolume() const override { return _volume; }

		operator Mix_Chunk* () const noexcept { return _soundChunk; }
	};

	class IPlayList: public IAudio
	{
	public:
		virtual ~IPlayList() = default;

		virtual void Shuffle() = 0;
		virtual std::span<Music> Items() = 0;
		virtual std::span<Music>::iterator Current() = 0;
	};
}