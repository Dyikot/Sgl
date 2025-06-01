#include "Playlist.h"

namespace Sgl
{
	Playlist::Playlist(const Playlist& other):
		Volume(other.Volume),
		Items(other.Items),
		_current(other._current)
	{}

	Playlist::Playlist(Playlist&& other) noexcept:
		Volume(other.Volume),
		Items(std::move(other.Items)),
		_current(std::move(other._current))
	{}

	void Playlist::Play()
	{
		if(Items.size() == 0)
		{
			return;
		}

		if(_current == Items.begin())
		{
			Started.TryRaise(*this, EmptyEventArgs);
		}
		else if(_current == Items.end())
		{
			Ended.TryRaise(*this, EmptyEventArgs);
			_current = Items.begin();
		}

		Music& current = *_current;

		if(!current.IsPlaying())
		{
			current.Play();
			_current++;
		}
		else if(current.IsPaused())
		{
			current.Resume();
		}
	}

	Playlist& Playlist::operator=(const Playlist& other)
	{
		Volume = other.Volume;
		Items = other.Items;
		_current = other._current;
		return *this;
	}

	Playlist& Playlist::operator=(Playlist&& other) noexcept
	{
		Volume = other.Volume;
		Items = std::move(other.Items);
		_current = std::move(other._current);
		return *this;
	}
}
