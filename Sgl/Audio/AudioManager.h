#pragma once

#include <list>
#include "Audio.h"
#include "Random/Random.h"

namespace Sgl
{	
	class AudioManager
	{
	public:
		Volume MasterVolume;
		Volume MusicVolume;
		Volume SoundEffectsVolume;		
		AudioResources* Resources = nullptr;
	protected:
		static constexpr int FreeChannel = -1;
	public:
		AudioManager();
		AudioManager(Volume masterVolume, Volume musicVolume, Volume soundEffectsVolume) noexcept;

		void PlayMusic(const Music& music, int loops = 0);
		void PlayMusic(const std::string& music, int loops = 0);
		void PlaySoundEffect(const SoundEffect& soundEffect, int channel = FreeChannel, int loops = 0);
		void PlaySoundEffect(const std::string& soundEffect, int channel = FreeChannel, int loops = 0);
		void PlayPlayList(PlayList& playlist);
		void PlayPlayList(const std::string& playlist);
		void PauseMusic() const noexcept;
		void ResumeMusic() const noexcept { Mix_ResumeMusic(); }
		void HaltMusic() const noexcept { Mix_HaltMusic(); }
		void SetMusicVolume(const Music& music);
		void SetMusicVolume(const Music& music, const AudioGroup& group);
		void SetSoundEffectVolume(const SoundEffect& soundEffect);
		void SetSoundEffectVolume(const SoundEffect& soundEffect, const AudioGroup& group);
	protected:
		uint8_t ToMixVolume(Volume volume, Volume audioTypeVolume, Volume groupVolume = Volume::Max) const
		{
			return (MasterVolume * audioTypeVolume * volume * groupVolume) 
				   / std::pow(Volume::Max, 4) * MIX_MAX_VOLUME;
		}
	};
}