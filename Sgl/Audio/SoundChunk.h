#pragma once

#include <memory>
#include <string_view>
#include "Volume.h"

namespace Sgl
{
	class SoundChunk
	{
	private:
		std::shared_ptr<Mix_Chunk> _soundChunk;
	public:
		SoundChunk() = default;
		SoundChunk(std::nullptr_t);
		explicit SoundChunk(std::string_view path) noexcept;
		SoundChunk(const SoundChunk& other);
		SoundChunk(SoundChunk&& other) noexcept;
		~SoundChunk() = default;

		void SetVolume(Volume value);
		Volume GetVolume() const;

		Mix_Chunk* ToMixChunk() const noexcept { return _soundChunk.get(); }
		
		SoundChunk& operator=(const SoundChunk&) = default;
		SoundChunk& operator=(SoundChunk&&) noexcept = default;
		friend bool operator==(const SoundChunk& left, const SoundChunk& right);
		explicit operator bool() const noexcept { return _soundChunk != nullptr; }
	};
}