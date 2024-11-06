#include "AudioManager.h"

namespace Sgl
{
	void AudioManager::SetVolume(size_t value)
	{
		SetSoundEffectsVolume(value);
		SetMusicVolume(value);
	}

	void AudioManager::SetSoundEffectsVolume(size_t value)
	{
		for(auto soundEffect : _soundEffects)
		{
			soundEffect->SetVolume(value);
		}
	}

	void AudioManager::SetMusicVolume(size_t value)
	{
		for(auto music : _musicTracks)
		{
			music->SetVolume(value);
		}
	}

	void AudioManager::PlayMusic()
	{
		if(_musicTracks.empty())
		{
			return;
		}
		
		if(!Mix_PlayingMusic())
		{
			if(_currentTrack == _musicTracks.end())
			{
				ShuffleMusicTracks();
			}

			CurrentTrack()->Play();
			_currentTrack++;
		}
		else if(Mix_PausedMusic())
		{
			CurrentTrack()->Resume();
		}
	}

	void AudioManager::ShuffleMusicTracks()
	{
		Random().Shuffle(_musicTracks);
		_currentTrack = _musicTracks.begin();
	}

	Music* const AudioManager::CurrentTrack() const noexcept
	{
		return _musicTracks.empty() ? nullptr : _currentTrack.operator*();
	}
}