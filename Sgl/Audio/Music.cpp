#include "Music.h"

namespace Sgl
{
	Music::Music(std::string_view path) noexcept
	{
		_music = Mix_LoadMUS(path.data());
	
		if(_music == nullptr)
		{
			std::cout << "Music: " << SDL_GetError() << '\n';
		}
	}

	Music::Music(Music&& other) noexcept
	{
		std::swap(_music, other._music);
	}

	Music::~Music() noexcept
	{
		Mix_FreeMusic(_music);
	}

	void Music::Play() const noexcept
	{
		Mix_PlayMusic(_music, PlayOnce);
	}

	void Music::Pause() const noexcept
	{
		if(!Mix_PausedMusic())
		{
			Mix_PauseMusic();
		}
	}

	void Music::Resume() const noexcept
	{
		Mix_ResumeMusic();
	}

	void Music::Stop() const noexcept
	{
		Mix_HaltMusic();
	}

	void Music::SetVolume(size_t value) noexcept
	{
		CorrectVolume(value);
		Mix_VolumeMusic(ToMixVolume(value));
	}
}