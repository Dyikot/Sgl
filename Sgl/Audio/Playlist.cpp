#include "Playlist.h"

namespace Sgl
{
	Playlist::Playlist(const Playlist& other):
		volume(other.volume),
		items(other.items),
		current(other.current)
	{}

	Playlist::Playlist(Playlist&& other) noexcept:
		volume(other.volume),
		items(std::move(other.items)),
		current(std::move(other.current))
	{}

	void Playlist::Play()
	{
		if(items.size() == 0)
		{
			return;
		}

		if(current == items.begin())
		{
			Started.TryRaise(*this, EventArgs());
		}
		else if(current == items.end())
		{
			Ended.TryRaise(*this, EventArgs());
			current = items.begin();
		}

		Music& music = *current;

		if(!music.IsPlaying())
		{
			music.Play();
			current++;
		}
		else if(music.IsPaused())
		{
			music.Resume();
		}
	}

	Playlist& Playlist::operator=(const Playlist& other)
	{
		volume = other.volume;
		items = other.items;
		current = other.current;
		return *this;
	}

	Playlist& Playlist::operator=(Playlist&& other) noexcept
	{
		volume = other.volume;
		items = std::move(other.items);
		current = std::move(other.current);
		return *this;
	}
}
