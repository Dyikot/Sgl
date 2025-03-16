#include "Audio.h"

namespace Sgl
{
	Music::Music(std::string_view path) noexcept:
		_music(Mix_LoadMUS(path.data()))
	{
		if(_music == nullptr)
		{
			Log::PrintSDLError();
		}
	}

	SoundEffect::SoundEffect(std::string_view path) noexcept:
		_soundChunk(Mix_LoadWAV(path.data()))
	{
		if(_soundChunk == nullptr)
		{
			Log::PrintSDLError();
		}
	}
}
