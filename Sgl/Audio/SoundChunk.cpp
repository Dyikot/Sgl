#include "SoundChunk.h"
#include "../Base/Log.h"

namespace Sgl
{
	struct SoundChunkDeleter
	{
		void operator()(Mix_Chunk* chunk) const
		{
			Mix_FreeChunk(chunk);
		}
	};

	SoundChunk::SoundChunk(std::nullptr_t)
	{}

	SoundChunk::SoundChunk(std::string_view path) noexcept:
		_soundChunk(Mix_LoadWAV(path.data()), SoundChunkDeleter())
	{
		Log::PrintSDLErrorIf(_soundChunk == nullptr);
	}

	SoundChunk::SoundChunk(const SoundChunk& other):
		_soundChunk(other._soundChunk)
	{}

	SoundChunk::SoundChunk(SoundChunk&& other) noexcept:
		_soundChunk(std::move(other._soundChunk))
	{}

	void SoundChunk::SetVolume(Volume value)
	{
		Mix_VolumeChunk(_soundChunk.get(), value.ToMixVolume());
	}

	Volume SoundChunk::GetVolume() const
	{
		return _soundChunk ? Volume(_soundChunk->volume) : Volume::Min;
	}

	bool operator==(const SoundChunk& left, const SoundChunk& right)
	{
		return left._soundChunk == right._soundChunk;
	}
}
