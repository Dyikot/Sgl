#include "AudioManager.h"

namespace Sgl
{
	AudioManager::AudioManager():
		AudioManager(Volume::Max, Volume::Max, Volume::Max)
	{}

	AudioManager::AudioManager(Volume masterVolume, Volume musicVolume, Volume soundEffectsVolume) noexcept:
		MasterVolume(masterVolume), MusicVolume(musicVolume), SoundEffectsVolume(soundEffectsVolume)
	{}
	void AudioManager::PlayMusic(const Music& music, int loops)
	{
		Mix_PlayMusic(music, loops);
	}

	void AudioManager::PlayMusic(const std::string& music, int loops)
	{
		if(Resources)
		{
			PlayMusic(*Resources->MusicTracks.at(music), loops);
		}
	}

	void AudioManager::PauseMusic() const noexcept
	{
		if(!Mix_PausedMusic())
		{
			Mix_PauseMusic();
		}
	}

	void AudioManager::SetMusicVolume(const Music& music)
	{
		Mix_VolumeMusic(ToMixVolume(music.Volume, MusicVolume, Volume::Max));
	}

	void AudioManager::SetSoundEffectVolume(const SoundEffect& soundEffect)
	{
		Mix_VolumeChunk(soundEffect,
						ToMixVolume(soundEffect.Volume, SoundEffectsVolume, Volume::Max));
	}

	void AudioManager::SetSoundEffectVolume(const SoundEffect& soundEffect, const AudioGroup& group)
	{
		Mix_VolumeChunk(soundEffect, 
						ToMixVolume(soundEffect.Volume, SoundEffectsVolume, group.Volume));
	}

	void AudioManager::PlaySoundEffect(const SoundEffect& soundEffect, int channel, int loops)
	{
		Mix_PlayChannel(channel, soundEffect, loops);
	}

	void AudioManager::PlaySoundEffect(const std::string& soundEffect, int channel, int loops)
	{
		if(Resources)
		{
			PlaySoundEffect(*Resources->SoundEffects.at(soundEffect), channel, loops);
		}
	}

	void AudioManager::PlayPlayList(PlayList& playlist)
	{
		if(playlist.empty())
		{
			return;
		}

		if(!Mix_PlayingMusic())
		{
			if(playlist.IsOver())
			{
				playlist.Shuffle();
			}
			
			SetMusicVolume(*playlist.Current(), playlist);
			PlayMusic(*playlist.Current());
			playlist.SetCurrentToNext();
		}
		else if(Mix_PausedMusic())
		{
			ResumeMusic();
		}
	}

	void AudioManager::PlayPlayList(const std::string& playlist)
	{
		if(Resources)
		{
			PlayPlayList(Resources->PlayLists.at(playlist));
		}
	}
}