#pragma once

#include <list>
#include "Audio.h"
#include "Random/Random.h"

namespace Sgl
{	
	class AudioManager
	{
	public:
		Volume MasterVolume = Volume::Max();
		Volume MusicVolume = Volume::Max();
		Volume SoundEffectsVolume = Volume::Max();
	private:
		static constexpr int Auto = -1;
	public:
		void Play(const Music& music, int loops = 0);
		void Play(const SoundEffect& soundEffect, int channel = Auto, int loops = 0);
		void Play(IPlayList& playlist);
		void PauseMusic() const noexcept;
		void ResumeMusic() const noexcept { Mix_ResumeMusic(); }
		void RewindMusic() const noexcept { Mix_RewindMusic(); }
		void HaltMusic() const noexcept { Mix_HaltMusic(); }
		void SetVolume(const SoundEffect& soundEffect);
		void SetVolume(const SoundEffect& soundEffect, Volume additional);
		void SetVolume(const Music& music);
		void SetVolume(const Music& music, Volume additional);
	};
}