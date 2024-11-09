#pragma once

#include <string_view>
#include <iostream>
#include "AudioBase.h"

namespace Sgl
{
	class Music: public AudioBase
	{
	protected:
		Mix_Music* _music;
	public:
		Music(std::string_view path) noexcept:
			_music(Mix_LoadMUS(path.data()))
		{
			if(_music == nullptr)
			{
				std::cout << "Music: " << SDL_GetError() << '\n';
			}
		}
		~Music() noexcept { Mix_FreeMusic(_music); }


		void Play() const noexcept override { Mix_PlayMusic(_music, Once); }
		void Pause() const noexcept 
		{
			if(!Mix_PausedMusic())
			{
				Mix_PauseMusic();
			} 
		}
		void Resume() const noexcept{ Mix_ResumeMusic(); }
		void Stop() const noexcept { Mix_HaltMusic(); }
		void SetVolume(Volume value) noexcept override { Mix_VolumeMusic(ToMixVolume(value)); }
	};
}