#pragma once

#include <string_view>
#include <iostream>
#include "AudioBase.h"

namespace Sgl
{
	class Music: public AudioBase
	{
	private:
		Mix_Music* _music;
	public:
		Music(std::string_view path) noexcept;
		Music(Music&& other) noexcept;	
		~Music() noexcept;

		void SetVolume(size_t value) noexcept override;

		void Play() const noexcept override;
		void Pause() const noexcept;
		void Resume() const noexcept;
		void Stop() const noexcept;
	};
}