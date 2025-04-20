#include "AudioManager.h"

namespace Sgl
{
	void AudioManager::Play(const Music& music, int loops)
	{
		Mix_PlayMusic(music, loops);
	}

	void AudioManager::PauseMusic() const noexcept
	{
		if(!Mix_PausedMusic())
		{
			Mix_PauseMusic();
		}
	}

	void AudioManager::SetVolume(const Music& music)
	{
		auto volume = MasterVolume * MusicVolume * music.GetVolume();
		Mix_VolumeMusic(volume.ToMixVolume());
	}

	void AudioManager::SetVolume(const Music& music, Volume additional)
	{
		auto volume = MasterVolume * MusicVolume * music.GetVolume() * additional;
		Mix_VolumeMusic(volume.ToMixVolume());
	}

	void AudioManager::SetVolume(const SoundEffect& soundEffect)
	{
		auto volume = MasterVolume * SoundEffectsVolume * soundEffect.GetVolume();
		Mix_VolumeChunk(soundEffect, volume.ToMixVolume());
	}

	void AudioManager::SetVolume(const SoundEffect& soundEffect, Volume additional)
	{
		auto volume = MasterVolume * SoundEffectsVolume * soundEffect.GetVolume() * additional;
		Mix_VolumeChunk(soundEffect, volume.ToMixVolume());
	}

	void AudioManager::Play(const SoundEffect& soundEffect, int channel, int loops)
	{
		Mix_PlayChannel(channel, soundEffect, loops);
	}

	void AudioManager::Play(IPlayList& playlist)
	{
		if(!Mix_PlayingMusic())
		{
			auto current = playlist.Current();
			auto items = playlist.Items();

			if(current != items.end())
			{
				playlist.Shuffle();
				current = items.begin();
			}
			
			SetVolume(*current, playlist.GetVolume());
			Play(*current);
			current++;
		}
		else if(Mix_PausedMusic())
		{
			ResumeMusic();
		}
	}
}