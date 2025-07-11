#include "Audio.h"

namespace Sgl
{
	Music::Music(std::string_view path) noexcept:
		_music(Mix_LoadMUS(path.data())),
		Duration(TimeSpan::FromSeconds(Mix_MusicDuration(_music)))
	{
		Log::PrintSDLErrorIf(_music == nullptr);
	}

	Music::~Music() noexcept
	{
		Mix_FreeMusic(_music);
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

	void Music::Resume() noexcept
	{
		Mix_ResumeMusic();
	}

	void Music::Rewind() noexcept
	{
		Mix_RewindMusic();
	}

	void Music::Halt() noexcept
	{
		Mix_HaltMusic();
	}

	bool Music::IsPaused() noexcept
	{
		return Mix_PausedMusic();
	}

	bool Music::IsPlaying() noexcept
	{
		return Mix_PlayingMusic();
	}

	SoundChunk::SoundChunk(std::string path) noexcept:
		_soundChunk(Mix_LoadWAV(path.data()))
	{
		Log::PrintSDLErrorIf(_soundChunk == nullptr);
	}

	SoundChunk::~SoundChunk() noexcept
	{
		Mix_FreeChunk(_soundChunk);
	}

	void SoundChunk::Play(int loops)
	{
		constexpr int freeChannel = -1;
		Play(freeChannel, loops);
	}

	void SoundChunk::Play(int channel, int loops) const
	{
		Mix_VolumeChunk(_soundChunk, Volume.ToMixVolume());
		Mix_PlayChannel(channel, _soundChunk, loops);
	}
}
