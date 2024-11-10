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

	void AudioManager::PlayMusic(const std::string & music, int loops)
	{
		if(Resources)
		{
			PlayMusic(*Resources->MusicTracks[music], loops);
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
		Mix_VolumeMusic(ToMixVolume(music.AudioVolume, MusicVolume, Volume::Max));
	}

	void AudioManager::SetMusicVolume(const Music& music, const AudioGroup& group)
	{
		Mix_VolumeMusic(ToMixVolume(music.AudioVolume, MusicVolume, group.Volume));
	}

	void AudioManager::SetSoundEffectVolume(const SoundEffect& soundEffect)
	{
		Mix_VolumeChunk(soundEffect,
						ToMixVolume(soundEffect.AudioVolume, SoundEffectsVolume, Volume::Max));
	}

	void AudioManager::SetSoundEffectVolume(const SoundEffect& soundEffect, const AudioGroup& group)
	{
		Mix_VolumeChunk(soundEffect,
						ToMixVolume(soundEffect.AudioVolume, SoundEffectsVolume, group.Volume));
	}

	void AudioManager::PlaySoundEffect(const SoundEffect& soundEffect, int channel, int loops)
	{
		Mix_PlayChannel(channel, soundEffect, loops);
	}

	void AudioManager::PlaySoundEffect(const std::string & soundEffect, int channel, int loops)
	{
		if(Resources)
		{
			PlaySoundEffect(*Resources->SoundEffects[soundEffect], channel, loops);
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

			PlayMusic(*playlist.Current());
			playlist.SetCurrentToNext();
		}
		else if(Mix_PausedMusic())
		{
			ResumeMusic();
		}
	}

	void AudioManager::PlayPlayList(const std::string & playlist)
	{
		if(Resources)
		{
			PlayPlayList(Resources->PlayLists[playlist]);
		}
	}
}