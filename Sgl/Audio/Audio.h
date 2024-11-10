#pragma once

#include <vector>
#include <string_view>
#include <iostream>
#include <unordered_map>
#include "SDL/SDL_mixer.h"

namespace Sgl
{
	class Volume
	{
	public:
		static constexpr uint8_t Max = 100;
	protected:
		uint8_t _value;
	public:
		Volume(uint8_t value):
			_value(Adjust(value))
		{}

		operator uint8_t() { return _value; }
		uint8_t operator*(Volume volume) const { return _value * volume._value; }
		uint8_t operator*(uint8_t value) const { return _value * value; }
	private:
		static uint8_t Adjust(uint8_t value) { return value > Max ? Max : value; }
	};

	class AudioBase
	{
	public:
		Volume AudioVolume;
	public:
		AudioBase(Volume audioVolume):
			AudioVolume(audioVolume)
		{}		
		virtual ~AudioBase() = default;
	};

	struct AudioGroup
	{
		Volume Volume;
		std::list<AudioBase*> Items;
	};

	class Music: public AudioBase
	{
	protected:
		Mix_Music* _music;
	public:
		Music(std::string_view path, Volume volume = Volume::Max) noexcept:
			_music(Mix_LoadMUS(path.data())), AudioBase(volume)
		{
			if(_music == nullptr)
			{
				std::cout << "Music: " << SDL_GetError() << '\n';
			}
		}
		~Music() noexcept { Mix_FreeMusic(_music); }

		operator Mix_Music* () const { return _music; }
	};

	class SoundEffect: public AudioBase
	{
	protected:
		Mix_Chunk* _soundChunk;
	public:
		SoundEffect(std::string_view path, Volume volume = Volume::Max) noexcept:
			_soundChunk(Mix_LoadWAV(path.data())), AudioBase(volume)
		{
			if(_soundChunk == nullptr)
			{
				std::cout << SDL_GetError() << '\n';
			}
		}
		~SoundEffect() noexcept { Mix_FreeChunk(_soundChunk); }

		operator Mix_Chunk* () const { return _soundChunk; }
	};	

	class PlayList: public std::vector<Music*>
	{
	private:
		PlayList::iterator _current;
	public:
		Music* const Current() const { return empty() ? nullptr : *_current; }
		void SetCurrentToBegin() { _current = begin(); }
		void SetCurrentToEnd() { _current = end(); }
		void SetCurrentToNext() { _current++; }
		void Shuffle();
		bool IsOver() const { return _current == end(); }
	};

	struct AudioResources
	{
		~AudioResources();

		std::unordered_map<std::string, SoundEffect*> SoundEffects;
		std::unordered_map<std::string, Music*> MusicTracks;
		std::unordered_map<std::string, AudioGroup> Groups;
		std::unordered_map<std::string, PlayList> PlayLists;
	};
}