#pragma once

#include <string_view>
#include <iostream>
#include "AudioBase.h"

namespace Sgl
{
	class SoundEffect: public AudioBase
	{
	protected:
		static constexpr int FreeChannel = -1;
		Mix_Chunk* _soundChunk;
	public:
		SoundEffect(std::string_view path) noexcept:
			_soundChunk(Mix_LoadWAV(path.data()))
		{
			if(_soundChunk == nullptr)
			{
				std::cout << SDL_GetError() << '\n';
			}
		}
		~SoundEffect() noexcept { Mix_FreeChunk(_soundChunk); }

		void SetVolume(Volume value) noexcept override { Mix_VolumeChunk(_soundChunk, ToMixVolume(value)); }
		void Play(int channel) const noexcept { Mix_PlayChannel(channel, _soundChunk, Once); }
		void Play() const noexcept override { Play(FreeChannel); }
	};
}