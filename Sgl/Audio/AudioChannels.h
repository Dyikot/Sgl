#pragma once

#include "SoundChunk.h"
#include "../Base/Time/TimeSpan.h"

namespace Sgl
{
	class AudioChannels
	{
	private:
		static constexpr int FreeChannel = -1;
		static constexpr int AllChannels = -1;
		static constexpr int Get = -1;
	public:
		static void Create(size_t newChannelsNumber)
		{
			Mix_AllocateChannels(newChannelsNumber);
		}

		static void SetMasterVolume(Volume value)
		{
			Mix_MasterVolume(value.ToMixVolume());
		}

		static Volume GetMasterVolume()
		{
			return Volume(static_cast<uint8_t>(Mix_MasterVolume(Get)));
		}

		static void SetVolume(size_t channel, Volume value)
		{
			Mix_Volume(channel, value.ToMixVolume());
		}

		static Volume GetVolume(size_t channel)
		{
			return Volume(static_cast<uint8_t>(Mix_Volume(channel, Get)));
		}

		static void Pause(size_t channel)
		{
			Mix_Pause(channel);
		}

		static void Pause()
		{
			Mix_Pause(AllChannels);
		}

		static bool IsPaused(size_t channel)
		{
			Mix_Paused(channel);
		}

		static bool IsPaused()
		{
			Mix_Paused(AllChannels);
		}

		static void Play(size_t channel, SoundChunk soundChunk, size_t loops = 0)
		{
			Mix_PlayChannel(channel, soundChunk.ToMixChunk(), loops);
		}

		static void Play(SoundChunk soundChunk, size_t loops = 0)
		{
			Mix_PlayChannel(FreeChannel, soundChunk.ToMixChunk(), loops);
		}

		static void PlayFor(size_t channel, SoundChunk soundChunk, TimeSpan duration, size_t loops = 0)
		{
			Mix_PlayChannelTimed(channel, soundChunk.ToMixChunk(), loops,
				duration.ToMilliseconds());
		}

		static void Play(SoundChunk soundChunk, TimeSpan duration, size_t loops = 0)
		{
			Mix_PlayChannelTimed(FreeChannel, soundChunk.ToMixChunk(), loops,
				duration.ToMilliseconds());
		}

		static void Resume(size_t channel)
		{
			Mix_Resume(channel);
		}

		static void Resume()
		{
			Mix_Resume(AllChannels);
		}

		static void Halt(size_t channel)
		{
			Mix_HaltChannel(channel);
		}

		static void Halt()
		{
			Mix_HaltChannel(AllChannels);
		}		

		static void SetDistance(size_t channel, uint8_t value)
		{
			Mix_SetDistance(channel, value);
		}
	};
}