#include "Audio.h"

namespace Sgl
{
	Music::Music(std::string_view path) noexcept:
		_music(Mix_LoadMUS(path.data())),
		Duration(TimeSpan::FromSeconds(Mix_MusicDuration(_music)))
	{
		PrintSDLErrorIf(_music == nullptr);
	}

	void Music::Play(int loops) const
	{
		Mix_VolumeMusic(Volume.ToMixVolume());
		Mix_PlayMusic(_music, loops);
	}

	void Music::Pause() noexcept
	{
		if(!Mix_PausedMusic())
		{
			Mix_PauseMusic();
		}
	}

	SoundChunk::SoundChunk(std::string_view path) noexcept:
		_soundChunk(Mix_LoadWAV(path.data()))
	{
		PrintSDLErrorIf(_soundChunk == nullptr);
	}

	void SoundChunk::Play(int channel, int loops) const
	{
		Mix_VolumeChunk(_soundChunk, Volume.ToMixVolume());
		Mix_PlayChannel(channel, _soundChunk, loops);
	}
}
