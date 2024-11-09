#include "AudioManager.h"

namespace Sgl
{
	AudioManager::~AudioManager()
	{
		for(auto& [_, musicTrack] :	_musicTracks)
		{
			delete musicTrack;
		}

		for(auto& [_, soundEffect] : _soundEffects)
		{
			delete soundEffect;
		}
	}

	void AudioManager::AddMusic(std::string_view path)
	{
		_musicTracks[path.data()] = new Music(path);
		_musicTracksOrder.push_back(_musicTracks[path.data()]);
	}

	void AudioManager::AddSoundEffect(std::string_view path)
	{
		_soundEffects[path.data()] = new SoundEffect(path);
	}

	void AudioManager::SetVolume(Volume value)
	{
		SetSoundEffectsVolume(value);
		SetMusicVolume(value);
	}

	void AudioManager::SetSoundEffectsVolume(Volume value)
	{
		for(auto& [_, soundEffect] : _soundEffects)
		{
			soundEffect->SetVolume(value);
		}
	}

	void AudioManager::SetMusicVolume(Volume value)
	{
		for(auto& [_, musicTrack] : _musicTracks)
		{
			musicTrack->SetVolume(value);
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
			if(_currentTrack == _musicTracksOrder.end())
			{
				Random().Shuffle(_musicTracksOrder);
				_currentTrack = _musicTracksOrder.begin();
			}

			CurrentTrack()->Play();
			_currentTrack++;
		}
		else if(Mix_PausedMusic())
		{
			CurrentTrack()->Resume();
		}
	}

	Music* AudioManager::CurrentTrack()  noexcept
	{
		return _musicTracks.empty() ? nullptr : *_currentTrack;
	}
}