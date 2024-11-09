#pragma once

#include <vector>
#include <unordered_map>
#include "Music.h"
#include "SoundEffect.h"
#include "Random/Random.h"

namespace Sgl
{
	class AudioManager
	{
	public:
		using MusicIterator = std::vector<Music*>::iterator;
	protected:
		std::unordered_map<std::string, Music*> _musicTracks;
		std::unordered_map<std::string, SoundEffect*> _soundEffects;
		std::vector<Music*> _musicTracksOrder;
		MusicIterator _currentTrack;
	public:
		~AudioManager();

		void AddMusic(std::string_view path);
		void AddSoundEffect(std::string_view path);
		Music& GetMusic(const std::string& name) { return *_musicTracks.at(name); }
		SoundEffect& GetSoundEffect(const std::string& name) { return *_soundEffects.at(name); }

		void SetVolume(Volume value);
		void SetSoundEffectsVolume(Volume value);
		void SetMusicVolume(Volume value);
		void PlayMusic();
		Music* CurrentTrack() noexcept;
	};
}