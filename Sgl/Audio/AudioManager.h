#pragma once

#include <vector>
#include "Music.h"
#include "SoundEffect.h"
#include "Random/Random.h"

namespace Sgl
{
	class AudioManager
	{
	protected:
		std::vector<Music*> _musicTracks;
		std::vector<SoundEffect*> _soundEffects;
		std::vector<Music*>::iterator _currentTrack;
	public:
		virtual ~AudioManager() = default;

		void SetVolume(size_t value);
		void SetSoundEffectsVolume(size_t value);
		void SetMusicVolume(size_t value);

		virtual void PlayMusic();
		virtual void ShuffleMusicTracks();
		Music* const CurrentTrack() const noexcept;
	};
}