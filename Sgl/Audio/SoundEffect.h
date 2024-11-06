#pragma once

#include <string_view>
#include <iostream>
#include "AudioBase.h"

namespace Sgl
{
	class SoundEffect: public AudioBase
	{
	private:
		static constexpr int ChannelAutoSelection = -1;

		Mix_Chunk* _soundChunk;
	public:
		SoundEffect(std::string_view path) noexcept;
		~SoundEffect() noexcept;

		void SetVolume(size_t value) noexcept override;

		void Play() const noexcept override;
		void Play(int channel) const noexcept;
	};
}