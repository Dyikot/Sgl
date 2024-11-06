#include "SoundEffect.h"

namespace Sgl
{
	SoundEffect::SoundEffect(std::string_view path) noexcept
	{
		_soundChunk = Mix_LoadWAV(path.data());

		if(_soundChunk == nullptr)
		{
			std::cout << SDL_GetError() << '\n';
		}
	}

	SoundEffect::~SoundEffect() noexcept
	{
		Mix_FreeChunk(_soundChunk);
	}

	void SoundEffect::Play() const noexcept
	{
		Mix_PlayChannel(ChannelAutoSelection, _soundChunk, PlayOnce);
	}

	void SoundEffect::Play(int channel) const noexcept
	{
		Mix_PlayChannel(channel, _soundChunk, PlayOnce);
	}

	void SoundEffect::SetVolume(size_t value) noexcept
	{
		CorrectVolume(value);
		Mix_VolumeChunk(_soundChunk, ToMixVolume(value));
	}
}