#pragma once

#include <vector>
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
		static constexpr float MaxValue = 1;
		float _value;
	public:
		explicit constexpr Volume(float value):
			_value(Adjust(std::abs(value)))
		{}

		int ToMixVolume() const
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
			_value = Adjust(_value * volume._value);
			return *this;
		}

		constexpr Volume& operator/=(Volume volume)
		{
			_value = Adjust(_value / volume._value);
			return *this;
		}
	private:
		static constexpr float Adjust(float value)
		{
			return value > MaxValue ? MaxValue : value;
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
			if(_music == nullptr)
			{
				Log::PrintSDLError();
			}
		}

		Music(const Music&) = delete;
		Music(Music&&) = delete;

		~Music() noexcept
		{ 
			Mix_FreeMusic(_music);
		}

		void SetVolume(Volume value) override { _volume = value; }
		Volume GetVolume() const override { return _volume; }

		operator Mix_Music* () const
		{ 
			return _music;
		}
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
			if(_soundChunk == nullptr)
			{
				Log::PrintSDLError();
			}
		}

		SoundEffect(const SoundEffect&) = delete;
		SoundEffect(SoundEffect&&) = delete;

		~SoundEffect() noexcept
		{ 
			Mix_FreeChunk(_soundChunk);
		}

		void SetVolume(Volume value) override { _volume = value; }
		Volume GetVolume() const override { return _volume; }

		operator Mix_Chunk* () const
		{
			return _soundChunk;
		}
	};
	
	class IPlayList: public IAudio
	{
	public:
		virtual ~IPlayList() = default;

		virtual Music* GetCurrent() const = 0;
		virtual void MoveCurrentToBegin() = 0;
		virtual void MoveCurrentToNext() = 0;
		virtual void Shuffle() = 0;
		virtual bool IsShuffleble() const = 0;
		virtual bool HasEnded() const = 0;
	};
}