#include "Audio.h"
#include <Random/Random.h>

namespace Sgl
{
	AudioResources::~AudioResources()
	{
		for(auto& [_, musicTrack] : MusicTracks)
		{
			delete musicTrack;
		}

		for(auto& [_, soundEffect] : SoundEffects)
		{
			delete soundEffect;
		}
	}

	void PlayList::Shuffle()
	{
		Random().Shuffle(*this);
		_current = begin();
	}
}
