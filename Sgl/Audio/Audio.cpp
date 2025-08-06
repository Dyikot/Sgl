#include "Audio.h"

namespace Sgl
{
	static constexpr int FreeChannel = -1;

	Music::Music(std::string_view path) noexcept:
		_music(Mix_LoadMUS(path.data()))
	{
		Log::PrintSDLErrorIf(_music == nullptr);
	}

	Music::Music(Music&& other) noexcept:
		_music(std::exchange(other._music, nullptr)),
		Volume(other.Volume)
	{}

	Music::~Music() noexcept
	{
		Mix_FreeMusic(_music);
	}

	Mix_Music* Music::GetMixMusic() const noexcept
	{
		return _music;
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

	bool Music::IsPaused() const noexcept
	{
		return Mix_PausedMusic();
	}

	bool Music::IsPlaying() const noexcept
	{
		return Mix_PlayingMusic();
	}

	TimeSpan Music::Duration() const noexcept
	{
		if(_music)
		{
			return TimeSpan::FromSeconds(Mix_MusicDuration(_music));
		}

		return TimeSpan::Zero;
	}

	Music& Music::operator=(Music&& other) noexcept
	{
		_music = std::exchange(other._music, nullptr);
		Volume = other.Volume;
		return *this;
	}

	SoundChunk::SoundChunk(std::string_view path) noexcept:
		_soundChunk(Mix_LoadWAV(path.data()))
	{
		Log::PrintSDLErrorIf(_soundChunk == nullptr);
	}

	SoundChunk::SoundChunk(SoundChunk&& other) noexcept:
		_soundChunk(std::exchange(other._soundChunk, nullptr)),
		Volume(other.Volume)
	{}

	SoundChunk::~SoundChunk() noexcept
	{
		Mix_FreeChunk(_soundChunk);
	}

	Mix_Chunk* SoundChunk::GetMixChunk() const noexcept
	{
		return _soundChunk;
	}

	void SoundChunk::Play(int loops)
	{		
		Play(FreeChannel, loops);
	}

	void SoundChunk::Play(int channel, int loops) const
	{
		Mix_VolumeChunk(_soundChunk, Volume.ToMixVolume());
		Mix_PlayChannel(channel, _soundChunk, loops);
	}

	SoundChunk& SoundChunk::operator=(SoundChunk&& other) noexcept
	{
		_soundChunk = std::exchange(other._soundChunk, nullptr);
		Volume = other.Volume;
		return *this;
	}
}
