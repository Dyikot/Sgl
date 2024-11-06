#include "AudioBase.h"

namespace Sgl
{
	AudioBase::AudioBase() noexcept
	{
		SetMaxVolume(MaxVolumeInPercent);
	}

	void AudioBase::SetMaxVolume(size_t value)
	{
		CorrectVolume(value);
		_partOfMaxVolume = value;
		_partOfMaxVolume /= MaxVolumeInPercent;
	}

	void AudioBase::CorrectVolume(size_t& volume)
	{
		if(volume > MaxVolumeInPercent)
		{
			volume = MaxVolumeInPercent;
		}
	}

	size_t AudioBase::ToMixVolume(size_t volume)
	{
		return volume * _partOfMaxVolume * MaxMixVolume / MaxVolumeInPercent;
	}
}