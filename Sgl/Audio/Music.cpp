#include "Music.h"
#include "../Base/Log.h"

namespace Sgl
{
	struct MusicDeleter
	{
		void operator()(Mix_Music* music) const
		{
			Mix_FreeMusic(music);
		}
	};	

	Music::Music(std::nullptr_t)
	{}

	Music::Music(std::string_view path) noexcept:
		_music(Mix_LoadMUS(path.data()), MusicDeleter())
	{
		Log::PrintSDLErrorIf(_music == nullptr);
	}

	Music::Music(const Music& other):
		_music(other._music)
	{}

	Music::Music(Music&& other) noexcept:
		_music(std::move(other._music))
	{}

	TimeSpan Music::Duration() const noexcept
	{
		if(_music)
		{
			return TimeSpan::FromMilliseconds(Mix_MusicDuration(_music.get()) * 1e3);
		}

		return TimeSpan::Zero;
	}

	bool operator==(const Music& left, const Music& right)
	{
		return left._music == right._music;
	}
}